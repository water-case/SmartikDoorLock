#include "etc.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];
extern char pub_msg[MAX_MSG_LEN];

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

void flash(int speed, int period) {										//speed의 속도로 period*1.2초동안
	pwm->set_period(pwm_pins[led0].handle,1000*1000*100*speed);			//speed가 1이면 엄청깜빡임, 10이면 1초주기로
	pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000*50*speed);		//

	while(1)
	{
		if(boolvariable.flash_timer==0)
		{
			boolvariable.flash_timer=1;
			artikvariable.delaytime=period*1000;			//period 5일때 6번깜빡임
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

void flashon()
{
	pwm->set_period(pwm_pins[led0].handle,1000);
	pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
}

void flashoff()
{
	pwm->set_period(pwm_pins[led0].handle,0);
	pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
}

void Open() {	//Open으로 바꿀예정

	bt->stop_scan();
	Sound("/root/open.wav");
	bt->unset_callback(BT_EVENT_SCAN);
	gpio->write(gpio_pins[motor0].handle,1);
	usleep(1000*300);
	gpio->write(gpio_pins[motor0].handle,0);
	boolvariable.door = open;
	intvariable.secretlongcount =0;
}

void Close() {			//Close으로 바꿀예정
	/*int value=0;
	adc->get_value(adc_pins[t_pin].handle,&value);
	printf("%d\n", value);
	if(boolvariable.door==open && value > 500 )
	{
		boolvariable.flg_tpin = true;
	}
	if(boolvariable.flg_tpin==true){
		if(boolvariable.t_timer==0) {
			artikvariable.t_initime = timer->get_tick();
			boolvariable.t_timer=1;
			artikvariable.delaytime=100;
		}
		artikvariable.newtime=timer->get_tick();
		if(artikvariable.newtime-artikvariable.t_initime>artikvariable.delaytime){
			if(value>500){
				gpio->write(gpio_pins[motor1].handle,1);
				usleep(1000*100);
				gpio->write(gpio_pins[motor1].handle,0);
				boolvariable.door = close;
				printf("close\n");

				charvariable.flg_door=0xD0;
				boolvariable.startInput=false;
				charvariable.detectcnt =0; 		//한번행동끝나면 다시하도록 초기화
			}
		}
	}*/
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
			charvariable.detectcnt =0; 		//한번행동끝나면 다시하도록 초기화
			boolvariable.keycheck = 0;
			boolvariable.bluecheck = 0;
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
		}

}

bool MADC() {
	int value=0;
	adc->get_value(adc_pins[m_pin].handle,&value);
	if(value>4000){
		return true;
	}
	else
		return false;
}

unsigned char getAbit(unsigned char x, unsigned char n) { // getbit()
  return (x & (1 << n)) >> n;
}

