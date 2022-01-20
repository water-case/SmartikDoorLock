#ifndef SOCKET_H_
#define SOCKET_H_

#include "main.h"

#define MAX_MSG_LEN	512
#define MAX_UUID_LEN	128

static char device_id[MAX_UUID_LEN] = "등록된 아틱모듈 아이디 입력하세요";
static char token[MAX_UUID_LEN] = "아틱클라우드 토큰값 입력하세요";


void websocket_connection_callback(void *user_data, void *result);
void websocket_receive_callback(void *user_data, void *result);
artik_error send_cloud_message( const char *msg );

#endif /* SOCKET_H_ */
