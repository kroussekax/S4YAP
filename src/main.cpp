#include <Arduino.h>
#include "BluetoothSerial.h"
#include "esp32-hal-ledc.h"

#define TOTAL_IR 5
#define paling_kiri 0
#define kiri 1
#define center 2
#define paling_kanan 3
#define kanan  4

#define KD 0
#define KI 1
#define KP 2

BluetoothSerial SerialBT;

int ir[TOTAL_IR] = {
	34,
	35,

	36,

	37, 
	38,
};

int enA=16, in1=27, in2=26;  // Motor A
int enB=18, in3=19, in4=23;  // Motor B

int freq=1000, resol=8;  // 1 kHz, 8-

float dt{};
unsigned long lastTime{};

float integral{};
float derivative{};

int error{};
int previous_error{};

float constants[3]; // kd ki kp
float calculatePID();

void calculate_error();
void receive_bluetooth();
void calculate_constant(String received_val);
void motor_code();

void setup() {
    Serial.begin(9600);

	SerialBT.begin("ESP32");


	for(int i=0;i<TOTAL_IR;i++){
		pinMode(ir[i], INPUT);
	}
	pinMode(in1,OUTPUT); pinMode(in2,OUTPUT);
	pinMode(in3,OUTPUT); pinMode(in4,OUTPUT);

	ledcAttach(enA, freq, resol);  // channel 0
	ledcAttach(enB, freq, resol);  // channel 1
}

void loop() {
	unsigned long now = micros();
	dt = (now - lastTime) * 1e-6;

	//receive_bluetooth();
	motor_code();

	lastTime = now;
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

void receive_bluetooth() {
	// Check if data has arrived over Bluetooth
	if (SerialBT.available()) {
		String received = "";

		// Read until newline (matches the '\n' the sender appends)
		received = SerialBT.readStringUntil('\n');
		received.trim();  // Strip any leftover whitespace/CR

		if(received.length() > 0) {
			Serial.print("Received value: ");
			Serial.println(received);

			// --- Do something with the value here ---
			int value = received.toInt();   // Convert to int if you're sending numbers
			Serial.print("As integer: ");
			Serial.println(value);
		}
	}
}

void calculate_constant(String received_val) {
	int index = received_val[0] - '0';
	float value{};
}

void motor_code() {
	digitalWrite(in1,HIGH); digitalWrite(in2,LOW);
	digitalWrite(in3,HIGH); digitalWrite(in4,LOW);
}

// getting the pid output
float calculatePID() {
	integral += error * dt;
	derivative = (error - previous_error) / dt;

	previous_error = error;
	return (constants[KP] * error) + (constants[KI] * integral) + (constants[KD] * derivative);
}
