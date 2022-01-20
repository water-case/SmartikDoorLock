#include "settinginitial.h"

artik_gpio_module *gpio = NULL;
artik_loop_module *loop = NULL;
artik_time_module *timer = NULL;
artik_pwm_module *pwm = NULL;
artik_adc_module *adc = NULL;
artik_bluetooth_module *bt = NULL;
artik_cloud_module *cl = NULL;


//gpio 핀
struct gpio_pins gpio_pins[] = {
	{ NULL, { KEY0, "key0", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL } },
	{ NULL, { KEY1, "key1", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL } },
	{ NULL, { KEY2, "key2", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL } },
	{ NULL, { KEY3, "key3", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL } },
	{ NULL, { KEY4, "key4", GPIO_IN, GPIO_EDGE_NONE, 1, NULL } },
	{ NULL, { KEY5, "key5", GPIO_IN, GPIO_EDGE_NONE, 1, NULL } },
	{ NULL, { KEY6, "key6", GPIO_IN, GPIO_EDGE_NONE, 1, NULL } },
	{ NULL, { MOTOR0, "motor0", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL } },
	{ NULL, { MOTOR1, "motor1", GPIO_OUT, GPIO_EDGE_NONE, 0, NULL } },
	{ NULL, { BUTTON, "button", GPIO_IN, GPIO_EDGE_NONE, 1, NULL } } };
//adc 핀
struct adc_pins adc_pins[] = {
	{ NULL, { m_pin, "red", NULL } },
	{ NULL, { t_pin, "red", NULL } },
	{ NULL, { b_pin, "red", NULL } },
	{ NULL, { d_pin, "red", NULL } }, };
//pwm핀
struct pwm_pins pwm_pins[] = {
	{NULL,{LED0,"led0",1000*1000,1000,ARTIK_PWM_POLR_NORMAL,NULL}} };

struct boolvariable boolvariable = { 0, 0, 0, 0, close, false, 0, 0, 0, 0, true, 0, 0, 0, 0 };

struct charvariable charvariable = {
		"", "",	"",	"    ",	"123456789*0",	"0123456789",	"    ",	"                 ",	"    ",	"    ",	"    ",	"    ",	"    ",	"    ", "    ",	0, 0, 0, 0 };

struct intvariable intvariable = {"", "", "", "", 0};

struct artikvariable artikvariable =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



void Settinginitial()
{
	int i;
	//사용할 모듈 설정
	Modulesetting();

	////////////////////////////////////////////////////////////////
	//여기부분 핀버튼 오류 해결위한 할당/할당해제함
	Assignment();
	Remove();
	//////////////////////////////////////////////////////////////
	//핀 할당
	Assignment();

	pwm->set_duty_cycle(pwm_pins[led0].handle,0); // 불꺼놓고시작


}

void Modulesetting()
{
	gpio = (artik_gpio_module *)artik_request_api_module("gpio");
	loop = (artik_loop_module *)artik_request_api_module("loop");
	timer = (artik_time_module *)artik_request_api_module("time");
	pwm = (artik_pwm_module *)artik_request_api_module("pwm");
	adc = (artik_adc_module *)artik_request_api_module("adc");
	bt = (artik_bluetooth_module *)artik_request_api_module("bluetooth");
	cl = (artik_cloud_module *)artik_request_api_module("cloud");

}

void Assignment()
{
	int i;
	//gpio 설정 할당
	for (i = 0; i < (sizeof(gpio_pins) / sizeof(*gpio_pins)); i++){
		gpio->request(&gpio_pins[i].handle, &gpio_pins[i].config);}
	//adc 설정
	for (i = 0; i < (sizeof(adc_pins) / sizeof(*adc_pins)); i++){
		adc->request(&adc_pins[i].handle, &adc_pins[i].config);}
	//pwm핀 설정 할당
	for (i = 0; i < (sizeof(pwm_pins) / sizeof(*pwm_pins)); i++){
		pwm->request(&pwm_pins[i].handle, &pwm_pins[i].config);}
}

void Remove()
{
	int i;

	for (i = 0; i < (sizeof(gpio_pins) / sizeof(*gpio_pins)); i++){
		gpio->release(gpio_pins[i].handle);}
	for (i = 0; i < (sizeof(adc_pins) / sizeof(*adc_pins)); i++){
			adc->release(adc_pins[i].handle);}
	for (i = 0; i < (sizeof(pwm_pins) / sizeof(*pwm_pins)); i++){
			pwm->release(pwm_pins[i].handle);}
}
