#include "main.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];

int main(void)
{		
	int i;
	// 초기 셋팅 함수
	Settinginitial();

	// 초기 아틱 설정
	artikvariable.initime=timer->get_tick();

	// 기기내 저장된 비밀번호 읽어오기
	FILE *fop;
	if( fop = fopen("/root/secretnum.txt", "r+"))
	{
		fscanf(fop, "%s", &charvariable.ONum);
	}
	fclose(fop);

	// 블루투스 인증키 초기화
	srand((unsigned)time(NULL));
	DeleteSNum();

	// 기기 메인 동작 루프문
	while(1)
	{
		artik_ssl_config ssl_config = {0};
		artik_websocket_handle handle;
		cl->websocket_open_stream(&handle, device_id, token, &ssl_config);
		cl->websocket_set_connection_callback(handle, websocket_connection_callback, &handle);
		loop->add_periodic_callback(&intvariable.calltime_MADC_id,100,LOOP,NULL);
		loop->run();
	}
	artik_release_api_module(loop);
	return 0;
}
