#pragma once

/*
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define TOTAL_IR 5
#define paling_kiri 0
#define kiri 1
#define center 2
#define paling_kanan 3
#define kanan  4

#define KD 0
#define KI 1
#define KP 2

inline int64_t lastTime = esp_timer_get_time();

inline int ir[TOTAL_IR] = {
	34,
	35,

	32,

	33,
	25, 
};

inline float constants[3]; // kd ki kp

inline int enA = 16, in1 = 27, in2 = 26;   // Motor A
inline int enB = 18, in3 = 19, in4 = 23;   // Motor B

inline int freq = 1000, resol = 8;

inline float error{0};
inline int previous_error{0};

inline float integral{0};
inline float derivative{0};

inline int lsp_base{0}, rsp_base{0};

inline bool start{false};

inline BluetoothSerial SerialBT;
inline int val, cnt = 0;
inline float v[3];

enum class BLUETOOTH_CODE {
	LeftSpeed,
	RightSpeed,
	Ki,
	Kd,
	Kp,
	Start
};
*/
