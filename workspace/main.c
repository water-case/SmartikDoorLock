#include "main.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];
///////////////////////////////////////////////////

int main(void)
{		
	int i;	//for�������������
	//�ʱ⼳��
	Settinginitial();	//settinginitial.c

	//������� ��ĵ �ݺ�����
	//bt->set_callback(BT_EVENT_SCAN, scan_callback, (void*)bt);

	//�ʱ�ƽ����
	artikvariable.initime=timer->get_tick();
	/////////////////////////////////////////////////////////////////////////////////////////
	//��������� ( ��й�ȣ �о���� , ����� �����ϱ� ���� �������������)
	FILE *fop;
	if( fop = fopen("/root/secretnum.txt", "r+"))
	{
		fscanf(fop, "%s", &charvariable.ONum);
	}
	fclose(fop);
	/////////////////////////////////////////////////////////////////////////////////////////
	//����Ű����
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
