#include <Arduino.h>

#define TOTAL_IR 5
#define paling_kiri 0
#define kiri 1
#define center 2
#define paling_kanan 3
#define kanan  4

#define KD 0
#define KI 1
#define KP 2

int ir[TOTAL_IR] = {
	34,
	35,

	36,

	37, 
	38,
};

float dt{};
unsigned long lastTime{};

float integral{};
float derivative{};

int error{};
int previous_error{};

float constants[3]; // kd ki kp
float calculatePID();

void calculate_error();
void calculate_pid();
void motor_code();

int enA = 16, in1 = 27, in2 = 26;   // Motor A
int enB = 18, in3 = 19, in4 = 23;   // Motor B

int freq = 1000, resol = 8;

enum DIRECTION{
	forward,
	left,
	right,
	stop
};

void motor_move(int dir, int speed);
void motor_move(int dir);

void setup() {
  Serial.begin(115200);

  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  pinMode(ir[0], INPUT); pinMode(ir[1], INPUT);

  ledcAttach(enA, freq, resol);
  ledcAttach(enB, freq, resol);

  ledcWrite(enA, 0);
  ledcWrite(enB, 0);

  delay(1000);
}

void loop() {
	motor_code();

	delay(10);
}

void motor_code() {
	bool left = digitalRead(ir[1]) == LOW;
	bool right = digitalRead(ir[0]) == LOW;

	if (!left && !right) motor_move(forward, 150);
	else if (left) motor_move(left, 180);
	else if (right) motor_move(right, 180);
	else motor_move(stop);
}

void motor_move(int dir) {
	motor_move(dir, 150);
}

void motor_move(int dir, int speed) {
	switch (dir) {
		case forward:
			{
				ledcWrite(enA, speed); digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
				ledcWrite(enB, speed); digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
			}
			break;
		case left:
			{
				ledcWrite(enA, speed); ledcWrite(enB, 0);
				digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
				digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
			}
			break;
		case right:
			{
				ledcWrite(enA, 0); ledcWrite(enB, speed);
				digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
				digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
			}
			break;
		case stop:
			{
				ledcWrite(enA, 0); ledcWrite(enB, 0);
				digitalWrite(in1, LOW); digitalWrite(in2, LOW);
				digitalWrite(in3, LOW); digitalWrite(in4, LOW);
			}
			break;
		default:
			break;
	}
}
