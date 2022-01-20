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
			charvariable.detectcnt =0; 		//�ѹ��ൿ������ �ٽ��ϵ��� �ʱ�ȭ
			boolvariable.bluecheck = 0;
			boolvariable.keycheck = 0;
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000);
		}
	}	
	if(boolvariable.door==close&&(charvariable.flg_door==0x00)){			//����ν� ����-���� �����ְ� �ƹ� ������ ���� ����
		if(MADC()==true){				//MADC ���� ����ν��� HIGH / LOW���� �Ǵ�
			printf("Motion Detected\n");
			if(boolvariable.inertblue==1)
				boolvariable.bluecheck=1;
			bt->start_scan(); //-> ��ĵ�� �Ϸ�Ǹ� ��������.
			bt->set_callback(BT_EVENT_SCAN, scan_callback, (void*)bt); // ��ĵ�ݹ�,

			charvariable.flg_door=0xD0; //����ν���� �÷���
			//flash(5,1);
			//originflash(3,300);
			pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);	 // ��ǵǸ� Ű�е� ������, ����
										 // ��������϶� �ݴ���ϵ���
		}
	}
	if(charvariable.flg_door==0x90||charvariable.flg_door==0x11||charvariable.flg_door==0x10) {	//Ű�ʱ�ȭ �۾�
		ResetKeyFlg();
	}else if(charvariable.flg_door==0xD0 && boolvariable.door==close ) {	// ����-����ν��� �Ȼ���
		//�⺻�����δ� Ű�е��Է��̰�, #���������� �����������Ű���
		if(boolvariable.startInput == 0) {
			//pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
			Check(1);
		}else {
			Check(0);
		}
		//Check(0); //������� ���Ű üũ
////////////////////////////////////////////////////////////////////////// ������������� Ŭ��������
	}else if(charvariable.flg_door==0x51) { //51-> ����ν��� ����������� ����Ȱ��
		pwm->set_duty_cycle(pwm_pins[led0].handle,1000*1000);
		charvariable.flg_door=0x50;
	}else if(charvariable.flg_door==0x50) {
		if(boolvariable.keytwinkle == 0) {
			printf("Key Click please~\n");
			boolvariable.keytwinkle = 1;
		}
		//Check(1); //��й�ȣüũ
	}

	if(boolvariable.bluecheck == 1 && boolvariable.keycheck == 1)
		charvariable.flg_door=0x30;	


	if((charvariable.flg_door==0x30)&&(boolvariable.door==close)) {
		Open();
		printf("Door open!! \n");
		originflash(10,30);
		charvariable.flg_door = 0x22; 		//���ʹޱ��� �׽�Ʈ��

		SendTime();				//����ð� ��������, ���ּ�����
		//�ð��� ������ Ŭ���忡 ����Ѵ�.	gpi07���� ������ 1

		printf("\n");
	}
			// ��й�ȣ�����ϱ��߰�
	if((charvariable.flg_door==0x22)&&(boolvariable.door==open)) {
		Close();		//������
	}

	return 1; // �������� �ֱ������� ����ǵ��� ����
}
void on_timeout_callback(void *user_data) {
	artik_loop_module *loop = (artik_loop_module *) user_data;
	loop->quit();
}
