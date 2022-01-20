#include "main.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];
///////////////////////////////////////////////////

int main(void)
{		
	int i;	//for구문에서사용함
	//초기설정
	Settinginitial();	//settinginitial.c

	//블루투스 스캔 콜벡설정
	//bt->set_callback(BT_EVENT_SCAN, scan_callback, (void*)bt);

	//초기틱설정
	artikvariable.initime=timer->get_tick();
	/////////////////////////////////////////////////////////////////////////////////////////
	//파일입출력 ( 비밀번호 읽어오기 , 저장및 변경하기 위해 파일입출력으로)
	FILE *fop;
	if( fop = fopen("/root/secretnum.txt", "r+"))
	{
		fscanf(fop, "%s", &charvariable.ONum);
	}
	fclose(fop);
	/////////////////////////////////////////////////////////////////////////////////////////
	//인증키만듬
	srand((unsigned)time(NULL));
	DeleteSNum();
	/////////////////////////////////////////////////////////////////////////////////////////
	while(1)
	{
		artik_ssl_config ssl_config = {0};
		artik_websocket_handle handle;
		cl->websocket_open_stream(&handle, device_id, token, &ssl_config);
		cl->websocket_set_connection_callback(handle, websocket_connection_callback, &handle);
		//cl->websocket_set_receive_callback(handle,websocket_receive_callback,NULL);
		//gpio->set_change_callback(gpio_pins[button].handle,interupt_button,NULL);
		//pwm->set_period(pwm_pins[led0].handle,1000);
		//pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
		loop->add_periodic_callback(&intvariable.calltime_MADC_id,100,LOOP,NULL);
		loop->run();
	}
	artik_release_api_module(loop);
	return 0;
}
