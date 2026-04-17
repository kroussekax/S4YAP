#include <Arduino.h>
#include "esp_timer.h"

#define TOTAL_IR 5

const int irPins[TOTAL_IR] = {34, 35, 32, 33, 25};
const int weights[TOTAL_IR] = {-2, -1, 0, 1, 2};

const int enA = 16;
const int in1 = 27;
const int in2 = 26;

const int enB = 18;
const int in3 = 19;
const int in4 = 23;

float Kp = 28.0;
float Ki = 0.0;
float Kd = 18.0;

float error = 0;
float previousError = 0;
float integral = 0;
float derivative = 0;

int baseLeft = 140;
int baseRight = 140;
int maxPWM = 255;

bool startRobot = true;
bool sensorActiveLow = true;

int64_t lastTime = 0;

void setMotorRaw(int pwmPin, int pin1, int pin2, int speed);
void setMotor(int leftSpeed, int rightSpeed);
float readLineError();
float calculatePID(float dt);
void stopMotor();

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  for (int i = 0; i < TOTAL_IR; i++) {
    pinMode(irPins[i], INPUT);
  }

  ledcAttach(enA, 1000, 8);
  ledcAttach(enB, 1000, 8);

  ledcWrite(enA, 0);
  ledcWrite(enB, 0);

  stopMotor();
  lastTime = esp_timer_get_time();
}

void loop() {
  int64_t now = esp_timer_get_time();
  float dt = (now - lastTime) / 1000000.0f;
  lastTime = now;

  if (dt <= 0) dt = 0.01;

  if (!startRobot) {
    stopMotor();
    delay(10);
    return;
  }

  error = readLineError();
  float pid = calculatePID(dt);

  int leftSpeed = (int)(baseLeft - pid);
  int rightSpeed = (int)(baseRight + pid);

  leftSpeed = constrain(leftSpeed, -maxPWM, maxPWM);
  rightSpeed = constrain(rightSpeed, -maxPWM, maxPWM);

  setMotor(leftSpeed, rightSpeed);

  Serial.print("Err: ");
  Serial.print(error);
  Serial.print(" PID: ");
  Serial.print(pid);
  Serial.print(" L: ");
  Serial.print(leftSpeed);
  Serial.print(" R: ");
  Serial.println(rightSpeed);

  delay(10);
}

float readLineError() {
  float sum = 0;
  int count = 0;

  for (int i = 0; i < TOTAL_IR; i++) {
    int raw = digitalRead(irPins[i]);
    bool active = sensorActiveLow ? (raw == LOW) : (raw == HIGH);

    if (active) {
      sum += weights[i];
      count++;
    }
  }

  if (count == 0) {
    if (previousError > 0) return 2.5;
    if (previousError < 0) return -2.5;
    return 0;
  }

  return sum / count;
}

float calculatePID(float dt) {
  integral += error * dt;
  integral = constrain(integral, -50.0, 50.0);

  derivative = (error - previousError) / dt;
  float output = (Kp * error) + (Ki * integral) + (Kd * derivative);

  previousError = error;
  return output;
}

void setMotorRaw(int pwmPin, int pin1, int pin2, int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    ledcWrite(pwmPin, speed);
  } else if (speed < 0) {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    ledcWrite(pwmPin, -speed);
  } else {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    ledcWrite(pwmPin, 0);
  }
}

void setMotor(int leftSpeed, int rightSpeed) {
  setMotorRaw(enA, in1, in2, leftSpeed);
  setMotorRaw(enB, in3, in4, rightSpeed);
}

void stopMotor() {
  setMotor(0, 0);
}
