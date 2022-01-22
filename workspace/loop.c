#include "loop.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];
extern char pub_msg[MAX_MSG_LEN];

// 프로그램 메인 루프
bool LOOP(){
	if(!gpio->read(gpio_pins[button].handle)){
		if(boolvariable.door==close)	{
			Open();
		}
		else{
			gpio->write(gpio_pins[motor1].handle,1);
			usleep(1000*300);
			gpio->write(gpio_pins[motor1].handle,0);
			boolvariable.door = close;
			printf("close\n");
			SendWrong(4);
			charvariable.flg_door=0x00;
			boolvariable.startInput=false;
			charvariable.detectcnt =0;
			boolvariable.bluecheck = 0;
			boolvariable.keycheck = 0;
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
		}
	}	
	/**
	*	적외선 센서를 이용한 움직임 검사
	*	움직임이 생기면 키패드에 불들어오면 조작이 가능해짐
	*	움직임이 생기면 블루투스 스캔시작하여 기기 탐색
	*/
	if(boolvariable.door==close&&(charvariable.flg_door==0x00)){
		if(MADC()==true){
			printf("Motion Detected\n");
			if(boolvariable.inertblue==1)
				boolvariable.bluecheck=1;
			bt->start_scan();
			bt->set_callback(BT_EVENT_SCAN, scan_callback, (void*)bt);

			charvariable.flg_door=0xD0
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
		}
	}
	if(charvariable.flg_door==0x90||charvariable.flg_door==0x11||charvariable.flg_door==0x10) {
		ResetKeyFlg();
	}
	/**
	*	모션인식이 된 상태일때 키패드입력모드와 블루투스인증모드 구분 비트에 따라 분기
	*/
	else if(charvariable.flg_door==0xD0 && boolvariable.door==close ) {
		if(boolvariable.startInput == 0) {
			Check(1);
		}else {
			Check(0);
		}
	}
	/**
	*	51 = 움직임 감지 이후 블루투스 인증이 통과된 경우 50으로 변경
	*/
	else if(charvariable.flg_door==0x51) {
		pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
		charvariable.flg_door=0x50;
	}
	/**
	*	50 = 블루투스 인증까지 된 경우
	*	일반키 모드로 변경
	*/
	else if(charvariable.flg_door==0x50) {
		if(boolvariable.keytwinkle == 0) {
			printf("Key Click please~\n");
			boolvariable.keytwinkle = 1;
		}
	}
	/**	
	*	블루투스 인증과 일반 비밀번호까지 통과했을 경우 flag를 30으로 변경
	*/
	if(boolvariable.bluecheck == 1 && boolvariable.keycheck == 1)
		charvariable.flg_door=0x30;	

	/**
	*	블루투스 인증과 일반 비밀번호까지 통과했을 경우 작업
	*/
	if((charvariable.flg_door==0x30)&&(boolvariable.door==close)) {
		Open(); // 문열기
		printf("Door open!! \n");
		originflash(10,30);
		charvariable.flg_door = 0x22; 

		SendTime(); // 현재시간과 MAC Address를 기기와 클라우드에 기록함

		printf("\n");
	}
	/**
	*	문열린 상태에서 문닫힘 버튼 누르면 문닫힘
	*/
	if((charvariable.flg_door==0x22)&&(boolvariable.door==open)) {
		Close();
	}

	return 1;
}

// 시간이 초과되면 루프를 끝냄
void on_timeout_callback(void *user_data) {
	artik_loop_module *loop = (artik_loop_module *) user_data;
	loop->quit();
}
