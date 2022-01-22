#include "keypad.h"

extern struct gpio_pins gpio_pins[10];
extern struct adc_pins adc_pins[4];
extern struct pwm_pins pwm_pins[1];
extern char pub_msg[MAX_MSG_LEN];

void Check(bool checknum) {
	int i;
	unsigned char key=0,row;
	for(row=0x01; row<0x10;row<<=1) {
		gpio->write(gpio_pins[key0].handle,!getAbit(row,0));
		gpio->write(gpio_pins[key1].handle,!getAbit(row,1));
		gpio->write(gpio_pins[key2].handle,!getAbit(row,2));
		gpio->write(gpio_pins[key3].handle,!getAbit(row,3));
		if(!gpio->read(gpio_pins[key4].handle)) {
			break;
		} key++;
		if(!gpio->read(gpio_pins[key5].handle)) {
			break;
		} key++;
		if(!gpio->read(gpio_pins[key6].handle)) {
			break;
		}key++;
	}
	gpio->write(gpio_pins[key0].handle,0);
	gpio->write(gpio_pins[key1].handle,0);
	gpio->write(gpio_pins[key2].handle,0);
	gpio->write(gpio_pins[key3].handle,0);

	/**
	*	입력받은 키를 따로 저장하고 *가 눌리면 블루투스 비밀번호 모드로 변환
	*/
	if(key==12) {
		if(boolvariable.keypad_timer==0) {
			artikvariable.keypad_initime = timer->get_tick();
			boolvariable.keypad_timer=1;
			artikvariable.keypad_delaytime=1000*3000;
		}
		artikvariable.keypad_newtime=timer->get_tick();
		if(artikvariable.keypad_newtime-artikvariable.keypad_initime>artikvariable.keypad_delaytime) {
			charvariable.flg_door=0x00;
			boolvariable.keypad_timer=0;
			printf("Time over!!!!\n");
			originflash(10,50);
			charvariable.inputleng=0;
			strcpy(&charvariable.inputSNum,"    ");
			strcpy(&charvariable.inputONum,"      ");
			intvariable.secretlongcount = 0;
			boolvariable.startInput=0;
			boolvariable.keytwinkle = 0;
			bt->stop_scan();
			bt->unset_callback(BT_EVENT_SCAN);
		}
	}
	else {
		artikvariable.keypad_initime = timer->get_tick();
		charvariable.flg_door &= ~0b01000000;
		// 블루투스 모드
		if(checknum == 0){
			if(key != 9) {
				if(charvariable.inputleng<4) {
					if(charvariable.inputleng !=3)
						Sound("/root/beep.wav");
					charvariable.inputSNum[charvariable.inputleng]=charvariable.keyPadMatrix[key];
					charvariable.inputleng++;
					printf("Clicked SNumber : %s\n", charvariable.inputSNum);
				}
				if(charvariable.inputleng==4) {
					// 블루투스 인증키를 요청한다
					if(!strcmp("0000",charvariable.inputSNum)) {
						MakeSNum();
						originflash(3,300);
						SendSNum(); // 블루투스 인증키 클라우드로 전송
						charvariable.inputleng=0;
						strcpy(&charvariable.inputSNum,"    ");
					}
					// 발급된 블루투스 인증키를 폐기한다
					else if(!strcmp("0001",charvariable.inputSNum)) {
						DeleteSNum();
						//flash(5,1);
						originflash(5,200);
						// 클라우드로 인증키 폐기를 알림
						SendWrong(1);
						charvariable.inputleng=0;
						strcpy(&charvariable.inputSNum,"    ");
						printf("resetSNum\n");
						printf("SNum : %s\n", charvariable.SNum);
						printf("wrong_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
					}
					// 블루트스 인증키 통과시
					else if(!strcmp(charvariable.SNum,charvariable.inputSNum)) {
						if(!boolvariable.keycheck)
							Sound("/root/pass.wav");
						bt->stop_scan();
						bt->unset_callback(BT_EVENT_SCAN);
						MakeSNum();
						for(i=0; i<4;i++)
							charvariable.name[i] = charvariable.SNumMatrix[9];
						SendBluep();
						printf("SNum sucess\n");
						charvariable.inputleng=0;
						strcpy(&charvariable.inputSNum,"    ");
						intvariable.secretlongcount=0;
						boolvariable.startInput=0;
						strcpy(charvariable.saveblue, "00000000000000000");
						printf("saveblue : %s\n", charvariable.saveblue);
						boolvariable.bluecheck = 1;
					}
					// 블루투스 인증키 3번이상 틀리면 클라우드로 경고알림 보냄
					else if( (charvariable.inputleng==4) && intvariable.secretlongcount == 2 ) {
						Sound("/root/longlonglongnum.wav");
						strcpy(pub_msg, "{\"wrong\":0002}");
						MakeSNum();
						originflash(10,50);
						SendWrong(2); // 클라우드로 경고알림 보냄
						charvariable.inputleng=0;
						strcpy(&charvariable.inputSNum,"    ");
						printf("SNum longlonglong!!!!\ncome back start!!!\n");
						intvariable.secretlongcount = 0;
						charvariable.flg_door = 0xD0;
					}
					/**
					*	블루투스 인증키 틀렸을 경우 경고 소리남
					*/
					else if( (charvariable.inputleng==4) && (strcmp(charvariable.SNum,charvariable.inputSNum) ) ) {
						charvariable.inputleng=0;
						strcpy(&charvariable.inputSNum,"    ");
						intvariable.secretlongcount++;
						printf("SNum long\n");
						if(intvariable.secretlongcount ==1)
							Sound("/root/longnum.wav");
						else
							Sound("/root/longlongnum.wav");
						originflash(5,80);
					}
				}
			}
			// 키패드의 #버튼으로 블루투스 모드와 일반 모드 변경함
			else {
				if(boolvariable.keycheck != 1) {
					Sound("/root/changenum.wav");
					boolvariable.startInput=0;
					pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
					charvariable.inputleng=0;
					strcpy(&charvariable.inputSNum,"    ");
				}
			}
		}
		// 일반 모드
		else if(checknum == 1) {
			if(key!=9 && charvariable.inputleng<6) {
				if(charvariable.inputleng != 5)
						Sound("/root/beep.wav");
				charvariable.inputONum[charvariable.inputleng]=charvariable.keyPadMatrix[key];
				charvariable.inputleng++;
				printf("Clicked ONumber : %s\n leng = %d\n", charvariable.inputONum, charvariable.inputleng);

				pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
			}else if(key == 9 && !boolvariable.bluecheck) {
				if(!boolvariable.startInput) {
				boolvariable.startInput=1;
				pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
				charvariable.inputleng=0;
				strcpy(&charvariable.inputONum,"      ");
				Sound("/root/changeblue.wav");
				}
			}

			if(charvariable.inputleng==6) {
				if(!strcmp(charvariable.ONum,charvariable.inputONum)) {
					if(!boolvariable.bluecheck)
						Sound("/root/pass.wav");
					printf("ONum sucess\n");
					charvariable.inputleng=0;
					boolvariable.keytwinkle = 0;
					strcpy(&charvariable.inputONum,"      ");
					pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
					boolvariable.keycheck = 1;
					boolvariable.startInput=1;
				}
				// 3번이상 틀리면 클라우드로 경고알림보냄
				else if( (charvariable.inputleng==6) && intvariable.secretlongcount == 2 ) {
					Sound("/root/longlonglongnum.wav");
					strcpy(pub_msg, "{\"wrong\":0003}");
					originflash(10,50);
					SendWrong(3);
					charvariable.inputleng=0;
					boolvariable.keytwinkle = 0;
					strcpy(&charvariable.inputONum,"      ");
					printf("ONum longlonglong!!!!\ncome back start!!!\n");
					charvariable.flg_door = 0xD0;
					intvariable.secretlongcount = 0;
					boolvariable.startInput=0;
				}
				// 비밀번호 틀렸을경우 경고소리남
				else if( (charvariable.inputleng==6) && (strcmp(charvariable.ONum,charvariable.inputONum) ) ) {
					if(intvariable.secretlongcount ==0)
						Sound("/root/longnum.wav");
					else
						Sound("/root/longlongnum.wav");
					charvariable.inputleng=0;
					boolvariable.keytwinkle = 0;
					strcpy(&charvariable.inputONum,"      ");
					intvariable.secretlongcount++;
					printf("ONum long\n");
					originflash(5,80);
				}
			}
		}
	}
}

// 문이 열릴때 바뀐 플래그를 닫힐때 플래그를 바꿈
void ResetKeyFlg(void) {
	if((gpio->read(gpio_pins[key4].handle)==1)&&(gpio->read(gpio_pins[key5].handle)==1)&&(gpio->read(gpio_pins[key6].handle)==1)) {
		charvariable.flg_door |= 0x40;
	}
}

