#include <Arduino.h>

#include "BluetoothSerial.h"

#define TOTAL_IR 5
#define paling_kiri 0
#define kiri 1
#define center 2
#define paling_kanan 3
#define kanan  4

#define KD 0
#define KI 1
#define KP 2

int64_t lastTime = esp_timer_get_time();

int ir[TOTAL_IR] = {
	34,
	35,

	32,

	25, 
	33,
};

float constants[3]; // kd ki kp
float calculatePID();

void calculate_error();
void calculate_pid();
void motor_both_wheel(int s_left, int s_right);
void motor_code(float dt);

int enA = 16, in1 = 27, in2 = 26;   // Motor A
int enB = 18, in3 = 19, in4 = 23;   // Motor B

int freq = 1000, resol = 8;

int error{0};
int previous_error{0};

int integral{0};
int derivative{0};

int lsp_base{0}, rsp_base{0};

bool start{false};

BluetoothSerial SerialBT;
int val, cnt = 0;
float v[3];

enum class BLUETOOTH_CODE {
	LeftSpeed,
	RightSpeed,
	Ki,
	Kd,
	Kp,
	Start
};

void read_bluetooth();
void process_bluetooth();

enum class DIRECTION{
	forward,
	left,
	right,
	stop
};

void motor_move(int dir, int speed);
void motor_move(int dir);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32");

  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  for(int i=0; i<TOTAL_IR; i++)
	pinMode(ir[i], INPUT);

  ledcAttach(enA, freq, resol);
  ledcAttach(enB, freq, resol);

  ledcWrite(enA, 0);
  ledcWrite(enB, 0);

  lsp_base = 150;
  rsp_base = 150;

  constants[KI] = 0;
  constants[KD] = 0;
  constants[KP] = 1;

  delay(1000);
}

void loop() {
	int64_t now = esp_timer_get_time();
	float dt = (now - lastTime) / 1e6f;

	if (SerialBT.available()){
		while(SerialBT.available() == 0);
		read_bluetooth();
		process_bluetooth();
	}

	if(start) motor_code(dt);

	delay(10);

	lastTime = now;
}

void read_bluetooth() {
	val = SerialBT.read();
	cnt++;
	v[cnt] = val;
	if (cnt == 2) cnt = 0;
}

void process_bluetooth() {
	switch((int)v[1]) {
		case (int)BLUETOOTH_CODE::LeftSpeed:
			lsp_base = v[2];
			break;
		case (int)BLUETOOTH_CODE::RightSpeed:
			rsp_base = v[2];
			break;
		case (int)BLUETOOTH_CODE::Kp:
			constants[KP] = v[2];
			break;
		case (int)BLUETOOTH_CODE::Ki:
			constants[KI] = v[2];
			break;
		case (int)BLUETOOTH_CODE::Kd:
			constants[KD] = v[2];
			break;
		case (int)BLUETOOTH_CODE::Start:
			start = !start;
			break;
		default:
			break;
	}
}

void calculate_error() {
	// error calculation methods
	// recheck for if its high or low later on pls dont forget this
	if(digitalRead(ir[paling_kiri]) == HIGH)
		error = -2;
	if(digitalRead(ir[kiri]) == HIGH)
		error = -1;
	if(digitalRead(ir[center]) == HIGH)
		error = 0;
	if(digitalRead(ir[kanan]) == HIGH)
		error = 1;
	if(digitalRead(ir[paling_kanan]) == HIGH)
		error = 2;
}

float calculate_pid(float dt) {
	integral += error * dt;
	derivative = (error - previous_error) / dt;

	previous_error = error;
	return (constants[KP] * error) + (constants[KI] * integral) + (constants[KD] * derivative);
}

void motor_code(float dt) {
	float lsp = lsp_base - calculate_pid(dt);
	float rsp = rsp_base + calculate_pid(dt);
    if (lsp > 255) {
      lsp = 255;
    }
    if (lsp < -255) {
      lsp = -255;
    }
    if (rsp > 255) {
      rsp = 255;
    }
    if (rsp < -255) {
      rsp = -255;
    }
	motor_both_wheel(lsp, rsp);
}

void motor_move(int dir) {
	motor_move(dir, 150);
}

void motor_both_wheel(int s_left, int s_right) {
	ledcWrite(enA, s_left); ledcWrite(enB, s_right);
	digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void motor_move(int dir, int speed) {
	switch (dir) {
		case (int)DIRECTION::forward:
			{
				ledcWrite(enA, speed); digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
				ledcWrite(enB, speed); digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
			}
			break;
		case (int)DIRECTION::left:
			{
				ledcWrite(enA, speed); ledcWrite(enB, 0);
				digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
				digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
			}
			break;
		case (int)DIRECTION::right:
			{
				ledcWrite(enA, 0); ledcWrite(enB, speed);
				digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
				digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
			}
			break;
		case (int)DIRECTION::stop:
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
