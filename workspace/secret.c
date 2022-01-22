#include "secret.h"

extern char pub_msg[MAX_MSG_LEN];

// 랜덤한 블루투스 인증키 생성
void MakeSNum(){
	int i, a;

	a=rand()%9+1;
	charvariable.SNum[0]=charvariable.SNumMatrix[a];

	for(i=1;i<4;i++){
		a=rand()%10;
		charvariable.SNum[i]=charvariable.SNumMatrix[a];
	}
	printf("makeSNum : %s\n", charvariable.SNum);
}

// 블루트스 인증키 폐기
void DeleteSNum(){
	int i;

	for(i=1;i<4;i++){
			charvariable.SNum[i]=NULL;
		}
}


void scan_callback(artik_bt_event event, void *data, void *user_data) {
	artik_bt_device *dev = (artik_bt_device *) data;
	FILE *fop;
	char* bluekey;
	printf("dev : %s \n",dev->remote_address);

	// 기기내 등록된 블루투스 MAC Address 목록 가져오기
	fop = fopen("/root/bluetoothkey.txt", "r+");

	// 기기 주변 블루투스 MAC Address 탐색
	while(fgets(bluekey,28,fop)) {
		if(strncmp(dev->remote_address,bluekey, sizeof(bluekey)) == 0) {
			bt->stop_scan();
			bt->unset_callback(BT_EVENT_SCAN);
			if(boolvariable.keycheck == 0)
				Sound("/root/punch.wav");
			charvariable.name[0] = bluekey[18];
			charvariable.name[1] = bluekey[19];
			charvariable.name[2] = bluekey[20];
			charvariable.name[3] = bluekey[21];
			SendBluep();
			printf("blue pass\n");
			boolvariable.blueo = 1;
			boolvariable.bluecheck = 1;
			break;
		}
	}
	if(boolvariable.blueo == 1){
		strcpy(charvariable.saveblue, bluekey);
		printf("saveblue : %s\n", charvariable.saveblue);
	}
	fclose(fop);
}
