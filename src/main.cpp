#include "globals.h"

#include "functions.h"

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32 test");

  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);

  for(int i=0; i<TOTAL_IR; i++)
	pinMode(ir[i], INPUT);

  ledcAttach(enA, freq, resol);
  ledcAttach(enB, freq, resol);

  ledcWrite(enA, 0);
  ledcWrite(enB, 0);

  lsp_base = 55;
  rsp_base = 55;

  constants[KI] = 0;
  constants[KD] = 0;
  constants[KP] = 50;

  delay(1000);
}

void loop() {
	int64_t now = esp_timer_get_time();
	float dt = (now - lastTime) / 1e6f;

	if (SerialBT.available()) {
		while(SerialBT.available() == 0);
		read_bluetooth();
		process_bluetooth();
	}
	if (start) motor_code(dt);

	delay(10);

	lastTime = now;
}
