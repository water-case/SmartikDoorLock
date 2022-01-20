#include "loop.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];
extern char pub_msg[MAX_MSG_LEN];

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
			charvariable.detectcnt =0; 		//한번행동끝나면 다시하도록 초기화
			boolvariable.bluecheck = 0;
			boolvariable.keycheck = 0;
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
		}
	}	
	if(boolvariable.door==close&&(charvariable.flg_door==0x00)){			//모션인식 조건-문이 닫혀있고 아무 동작이 없는 상태
		if(MADC()==true){				//MADC 지금 모션인식이 HIGH / LOW인지 판단
			printf("Motion Detected\n");
			if(boolvariable.inertblue==1)
				boolvariable.bluecheck=1;
			bt->start_scan(); //-> 스캔이 완료되면 없에야함.
			bt->set_callback(BT_EVENT_SCAN, scan_callback, (void*)bt); // 스캔콜백,

			charvariable.flg_door=0xD0; //모션인식통과 플래그
			//flash(5,1);
			//originflash(3,300);
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);	 // 모션되면 키패드 켜지고, 깜빡
										 // 블루투스일땐 반대로하도록
		}
	}
	if(charvariable.flg_door==0x90||charvariable.flg_door==0x11||charvariable.flg_door==0x10) {	//키초기화 작업
		ResetKeyFlg();
	}else if(charvariable.flg_door==0xD0 && boolvariable.door==close ) {	// 조건-모션인식이 된상태
		//기본적으로는 키패드입력이고, #눌렀을때는 블루투스인증키모드
		if(boolvariable.startInput == 0) {
			//pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
			Check(1);
		}else {
			Check(0);
		}
		//Check(0); //블루투스 통과키 체크
////////////////////////////////////////////////////////////////////////// 블루투스인증인 클라우드전송
	}else if(charvariable.flg_door==0x51) { //51-> 모션인식후 블루투스인증 통과된경우
		pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
		charvariable.flg_door=0x50;
	}else if(charvariable.flg_door==0x50) {
		if(boolvariable.keytwinkle == 0) {
			printf("Key Click please~\n");
			boolvariable.keytwinkle = 1;
		}
		//Check(1); //비밀번호체크
	}

	if(boolvariable.bluecheck == 1 && boolvariable.keycheck == 1)
		charvariable.flg_door=0x30;	


	if((charvariable.flg_door==0x30)&&(boolvariable.door==close)) {
		Open();
		printf("Door open!! \n");
		originflash(10,30);
		charvariable.flg_door = 0x22; 		//모터달기전 테스트용

		SendTime();				//현재시간 가져오고, 맥주소저장
		//시간을 가져와 클라우드에 기록한다.	gpi07번이 열릴때 1

		printf("\n");
	}
			// 비밀번호변경하기추가
	if((charvariable.flg_door==0x22)&&(boolvariable.door==open)) {
		Close();		//문닫힘
	}

	return 1; // 루프에서 주기적으로 실행되도록 설정
}
void on_timeout_callback(void *user_data) {
	artik_loop_module *loop = (artik_loop_module *) user_data;
	loop->quit();
}
