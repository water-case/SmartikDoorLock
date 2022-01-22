#include "etc.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];
extern char pub_msg[MAX_MSG_LEN];

// 기기 내부적으로 특정 행동을 진행중에도 버튼을 누를 수 있도록 하는 인터럽트함수
void interupt_button(){
	while(1)
	{
		printf("\n\n1\n");
		if(boolvariable.interupt_timer==0) {
			boolvariable.interupt_timer=1;
			artikvariable.interupt_delaytime=1;
			artikvariable.interupt_initime = timer->get_tick();
		}
		artikvariable.interupt_newtime=timer->get_tick();
		if(artikvariable.interupt_newtime-artikvariable.interupt_initime>artikvariable.interupt_delaytime) {
			boolvariable.interupt_timer=0;
			if(!gpio->read(gpio_pins[button].handle)){
				if(boolvariable.button_flg)
				{
					while(1){
						if(boolvariable.interupt_timer==0) {
							artikvariable.interupt_initime = timer->get_tick();
							boolvariable.interupt_timer=1;
							artikvariable.interupt_delaytime=50;
							gpio->write(gpio_pins[motor0].handle,1);
						}
						artikvariable.interupt_newtime=timer->get_tick();
						if(artikvariable.interupt_newtime-artikvariable.interupt_initime>artikvariable.interupt_delaytime){
							gpio->write(gpio_pins[motor0].handle,0);
							break;
						}
					}
					boolvariable.button_flg =! boolvariable.button_flg;
					break;
				}
				if(!boolvariable.button_flg)
				{
					while(1){
						if(boolvariable.interupt_timer==0) {
							artikvariable.interupt_initime = timer->get_tick();
							boolvariable.interupt_timer=1;
							artikvariable.interupt_delaytime=50;
							gpio->write(gpio_pins[motor1].handle,1);
						}
						artikvariable.interupt_newtime=timer->get_tick();
						if(artikvariable.interupt_newtime-artikvariable.interupt_initime>artikvariable.interupt_delaytime){
							gpio->write(gpio_pins[motor1].handle,0);
							break;
						}
					}
					boolvariable.button_flg =! boolvariable.button_flg;
					break;
				}
			}
		}
	}
}

/**
*	speed의 속도로 period*1.2초 간 키패드가 깜빡임
*	speed가 10일때 1초주기로 깜빡임
*/
void flash(int speed, int period) {
	pwm->set_period(pwm_pins[led0].handle,1000*1000*100*speed);
	pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000*50*speed);

	while(1)
	{
		if(boolvariable.flash_timer==0)
		{
			boolvariable.flash_timer=1;
			artikvariable.delaytime=period*1000;
			artikvariable.flash_initime = timer->get_tick();
		}
		artikvariable.newtime=timer->get_tick();

		if(artikvariable.newtime-artikvariable.flash_initime>artikvariable.delaytime)
		{
			pwm->set_period(pwm_pins[led0].handle,1000);
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
			boolvariable.flash_timer=0;

			break;
		}
	}
}

/**
*	키패드를 깜빡거리게 하는 함수
*/
void originflash(int num, int n)
{
	int i;
	for(i=0; i<num; i++)
	{
		pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
		usleep(1000*n);
		pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
		usleep(1000*n);
	}
}

/**
*	키패드의 불을 켜는 함수
*/
void flashon()
{
	pwm->set_period(pwm_pins[led0].handle,1000);
	pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
}

/**
*	키패드의 불을 끄는 함수
*/
void flashoff()
{
	pwm->set_period(pwm_pins[led0].handle,0);
	pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
}

/**
*	도어락이 열렸을때 키패드를 깜빡거리며 알림음이 나오도록 하는 함수
*/
void Open() {

	bt->stop_scan();
	Sound("/root/open.wav");
	bt->unset_callback(BT_EVENT_SCAN);
	gpio->write(gpio_pins[motor0].handle,1);
	usleep(1000*300);
	gpio->write(gpio_pins[motor0].handle,0);
	boolvariable.door = open;
	intvariable.secretlongcount =0;
}

/**
*	도어락을 닫는 함수
*	닫힐때 클라우드로 닫힘 메시지 전송함
*/
void Close() {
	if(!gpio->read(gpio_pins[button].handle))
		{
			gpio->write(gpio_pins[motor1].handle,1);
			usleep(1000*300);
			gpio->write(gpio_pins[motor1].handle,0);
			boolvariable.door = close;
			printf("close\n");
			SendWrong(4);
			charvariable.flg_door=0x00;
			boolvariable.startInput=false;
			charvariable.detectcnt =0;
			boolvariable.keycheck = 0;
			boolvariable.bluecheck = 0;
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
		}

}

/**
*	적외선 센서값을 분석하는 함수
*	도어락 앞에 움직임이 감지되면 블루투스 탐색과 도어락키패드를 조작할 수 있음
*	문앞 움직임이 없으면 도어락키패드를 조작할 수 없기 때문에
*	원격으로 도어락키패드 조작이 불가능하도록 구현함
*/
bool MADC() {
	int value=0;
	adc->get_value(adc_pins[m_pin].handle,&value);
	if(value>4000){
		return true;
	}
	else
		return false;
}

/**
*	도어락의 여러곳에서 flag를 이용하여 동작을 구분함
*	flag변수의 특정 비트의 값을 알기위해 만든 함수
*/
unsigned char getAbit(unsigned char x, unsigned char n) {
  return (x & (1 << n)) >> n;
}

