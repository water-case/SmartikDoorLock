#ifndef SETTINGINITIAL_H_
#define SETTINGINITIAL_H_

#include "main.h"

#define KEY0		128		//GPIO0
#define KEY1		129		//GPIO1
#define KEY2		130		//GPIO2
#define KEY3		46		//GPIO3
#define KEY4		14		//GPIO4
#define KEY5		41		//GPIO5
#define KEY6		25		//GPIO6
#define MOTOR0		0//0		//GPIO7
#define MOTOR1		26//26		//GPIO8
#define BUTTON		27//27			//GPIO9
// ADC는 12bits -> 0~4095
#define M_PIN		0	// 모션 센서
#define T_PIN		1	// 접점 센서
#define B_PIN		2	// 문열림 버튼 센서
#define D_PIN		3	// 문 닫힘 탐지센서
// PWM 핀 사용
#define LED0		((0 << 8) | 2)	// 키패드 LED
#define close		0
#define open		1

enum {
	key0=0,		// GPIO0 키패드 신호선
	key1,		// GPIO1
	key2,
	key3,
	key4,
	key5,
	key6,		// GPIO6
	motor0,		// GPIO7 문열림
	motor1,		// GPIO8 문닫힘
	button		// GPIO9 스위치
};
enum{
	m_pin,	 	// ADC0 모션 센서
	t_pin,		// ADC1 기울기 센서
	b_pin,		// ADC2 문열림 버튼
	d_pin,		// ADC3 문닫힘 감지 센서
};
enum{
	led0		// pwm0 키패드 LED
};

struct gpio_pins {
	artik_gpio_handle handle;
	artik_gpio_config config;
};
struct adc_pins{
	artik_adc_handle handle;
	artik_adc_config config;
};
struct pwm_pins{
	artik_pwm_handle handle;
	artik_pwm_config config;
};

struct boolvariable{
	bool interupt_timer;
	bool flash_timer;
	bool keypad_timer;
	bool t_timer;
	bool door;
	bool startInput;
	bool keytwinkle;
	bool secret;
	bool request;
	bool blueo;
	bool button_flg;
	bool flg_tpin;
	bool bluecheck;
	bool keycheck;
	bool inertblue;
};

struct charvariable{
	char inputSNum[5];
	char inputONum[7];
	char ONum[7];
	char SNum[5];
	char keyPadMatrix[12];
	char SNumMatrix[11];
	char name[5];
	char saveblue[18];
	char year[5], month[5], day[5], hours[5], minut[5], secon[5];
	char wrong[5];

	unsigned char keycnt, detectcnt,inputleng;
	unsigned char flg_door;
};

struct intvariable{
	int timeout_id;
	int calltime_id;
	int calltime_MADC_id;
	int calltime_cloud_id;

	int secretlongcount;
};

struct artikvariable{
	artik_msecond keypad_newtime, keypad_delaytime, keypad_initime;
	artik_msecond initime, newtime, delaytime;
	artik_msecond interupt_initime, interupt_newtime, interupt_delaytime;
	artik_msecond flash_initime;
	artik_msecond t_initime;
};


void Modulesetting();
void Settinginitial();
void Assignment();
void Remove();

#endif /* SETTINGINITIAL_H_ */
