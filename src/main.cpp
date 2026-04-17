#include <Arduino.h>

int enA = 16, in1 = 27, in2 = 26;   // Motor A
int enB = 18, in3 = 19, in4 = 23;   // Motor B

int freq = 1000, resol = 8;

void setup() {
  Serial.begin(115200);

  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  ledcAttach(enA, freq, resol);
  ledcAttach(enB, freq, resol);

  ledcWrite(enA, 0);
  ledcWrite(enB, 0);

  delay(1000);
}

void loop() {
  // Maju
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  ledcWrite(enA, 180);
  ledcWrite(enB, 180);
  delay(3000);

  // Stop
  ledcWrite(enA, 0);
  ledcWrite(enB, 0);
  delay(1500);

  // Mundur
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  ledcWrite(enA, 180);
  ledcWrite(enB, 180);
  delay(3000);

  // Stop
  ledcWrite(enA, 0);
  ledcWrite(enB, 0);
  delay(1500);
}
