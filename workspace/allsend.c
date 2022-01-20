#include "allsend.h"

extern char pub_msg[MAX_MSG_LEN];

void SendSNum(){
	strcpy(pub_msg, "{\"state\":    }");
	strncpy(pub_msg+9, charvariable.SNum, 4);

	printf("sendSNum : %s\n", charvariable.SNum);
	printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
	send_cloud_message(pub_msg);
}


void SendBluep()
{
	strcpy(pub_msg, "{\"bluep\":    }");
	strncpy(pub_msg+9, charvariable.name, 4);

	printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
	send_cloud_message(pub_msg);
}

void SendWrong(int a)
{
	int i;
	strcpy(pub_msg, "{\"wrong\":    }");

	for(i=0;i<4;i++)
			charvariable.wrong[i]=charvariable.SNumMatrix[a];
	strncpy(pub_msg+9, charvariable.wrong, 4);

	printf("pub_msg : %s,  size : %d\n", pub_msg, sizeof(pub_msg));
	send_cloud_message(pub_msg);

		//strcpy(pub_msg, );
	//else if(a==2) send_cloud_message("{\"wrong\":0002}");
		//strcpy(pub_msg, "{\"wrong\":0002}");
	//else if(a==3) send_cloud_message("{\"wrong\":0003}");
		//strcpy(pub_msg, "{\"wrong\":0003}");
	//else
		//strcpy(pub_msg, "{\"wrong\":0000}");

	/*strcpy(pub_msg, "{\"wrong\":0000}");
	strncpy(pub_msg+12, a , 1);*/


	//send_cloud_message(pub_msg);
}

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


	FILE *fop;											//문개폐시간, 블루투스맥주소 기기내 저장
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
