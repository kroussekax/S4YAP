#include "functions.h"
/*

#include "globals.h"

void read_bluetooth() {
	val = SerialBT.read();
	Serial.write(val);
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
	float sum = 0;
	int count = 0;

	int weights[TOTAL_IR] = {-2, -1, 0, 1, 2};

	for (int i = 0; i < TOTAL_IR; i++) {
		if (digitalRead(ir[i]) == HIGH) {
			sum += weights[i];
			count++;
		}
	}

	if (count == 0) {
		// no sensor active, keep previous error
		// so the robot remembers which way to turn
		return;
	}

	error = sum / count;  // weighted average
}

float calculate_pid(float dt) {
	integral += error * dt;
	derivative = (error - previous_error) / dt;

	previous_error = error;
	return (constants[KP] * error) + (constants[KI] * integral) + (constants[KD] * derivative);
}

void motor_code(float dt) {
	if (dt <= 0) return;

	calculate_error();
	float pid = calculate_pid(dt);

	float lsp = lsp_base - pid;
	float rsp = rsp_base + pid;

	lsp = constrain(lsp, -255, 255);
	rsp = constrain(rsp, -255, 255);

	motor_both_wheel(rsp, lsp);
}

void motor_move(int dir) {
	motor_move(dir, 150);
}

void motor_both_wheel(int s_left, int s_right) {
	ledcWrite(enA, s_left); ledcWrite(enB, s_right);
	digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
	digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}
*/
