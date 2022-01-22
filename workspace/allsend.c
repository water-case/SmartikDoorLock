#include "allsend.h"

extern char pub_msg[MAX_MSG_LEN];

/**
*	블루투스 임시비밀번호를 클라우드로 전송함
*	클라우드에 임시비밀번호 전송시 트리거로 등록된 카카오톡 또는 라인들로 임시비밀번호가 전송됨
*/
void SendSNum(){
	strcpy(pub_msg, "{\"state\":    }");
	strncpy(pub_msg+9, charvariable.SNum, 4);

	printf("sendSNum : %s\n", charvariable.SNum);
	printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
	send_cloud_message(pub_msg);
}

/**
*	블루투스 임시비밀번호로 입장할경우 일반 입장과는 다른 성공메시지를 클라우드로 보냄
*	일반 성공시 카카오톡과 라인으로 전송되는 메시지에 구별되어짐
*/
void SendBluep()
{
	strcpy(pub_msg, "{\"bluep\":    }");
	strncpy(pub_msg+9, charvariable.name, 4);

	printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
	send_cloud_message(pub_msg);
}

/**
*	비밀번호를 3번틀리면 클라우드로 경고 전송
*	클라우드에 경고알림 도착시 트리거로 등록된 카카오톡 또는 라인들로 경고 알림이 전송됨
*	
*	*특이사항으로 함수 파라미터로 1이 전송되면 인증키 폐기라는 특수한 행동하도록 구현함
*/
void SendWrong(int a)
{
	int i;
	strcpy(pub_msg, "{\"wrong\":    }");

	for(i=0;i<4;i++)
			charvariable.wrong[i]=charvariable.SNumMatrix[a];
	strncpy(pub_msg+9, charvariable.wrong, 4);

	printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
	send_cloud_message(pub_msg);
}

/**
*	입출입 기록을 클라우드로 전송함
*	클라우드로 전송시 트리거로 등록된 카카오톡 또는 라인들로 알림전송
*	알림에는 입출입시간 + 등록된 기기정보를 활용한 사용자이름이 전송됨
*/
void SendTime()
{
	int a;
	time_t ltime;

	struct tm *today;
    time(&ltime);
    today = localtime(&ltime);

    printf("%02d:%02d:%02d\n", today->tm_hour, today->tm_min, today->tm_sec);

	a=today->tm_hour / 10;
	charvariable.hours[2]=charvariable.SNumMatrix[a];
	a=today->tm_hour % 10;
	charvariable.hours[3]=charvariable.SNumMatrix[a];
	a=today->tm_min / 10;
	charvariable.minut[2]=charvariable.SNumMatrix[a];
	a=today->tm_min % 10;
	charvariable.minut[3]=charvariable.SNumMatrix[a];
	a=today->tm_sec / 10;
	charvariable.secon[2]=charvariable.SNumMatrix[a];
	a=today->tm_sec % 10;
	charvariable.secon[3]=charvariable.SNumMatrix[a];

	if(charvariable.hours[2]==charvariable.SNumMatrix[0])
	{
		strcpy(pub_msg, "{\"secon\":00000}");
		strncpy(pub_msg+9, charvariable.hours+3, 1);
		strncpy(pub_msg+10, charvariable.minut+2, 2);
		strncpy(pub_msg+12, charvariable.secon+2, 2);
		printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
		send_cloud_message(pub_msg);
	}else
	{
		strcpy(pub_msg, "{\"secon\":000000}");
		strncpy(pub_msg+9, charvariable.hours+2, 2);
		strncpy(pub_msg+11, charvariable.minut+2, 2);
		strncpy(pub_msg+13, charvariable.secon+2, 2);
		printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
		send_cloud_message(pub_msg);
	}

	// 입출입 기록을 기기내에도 저장함
	FILE *fop;
	fop = fopen("/root/doorlockdocument.txt", "a+");
	fprintf(fop, "TIME %04d-%02d-%02d %02d:%02d:%02d \nBLUETOOTHMAC %s \n\n",
			today->tm_year + 1900,
			today->tm_mon + 1,
			today->tm_mday,
			today->tm_hour,
			today->tm_min,
			today->tm_sec,
			charvariable.saveblue );
	fclose(fop);
}
