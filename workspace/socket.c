#include "socket.h"

char pub_msg[MAX_MSG_LEN] = "{\"state\":0000}";

///websocke///
void websocket_connection_callback(void *user_data, void *result)
{
    intptr_t connected = (intptr_t)result;
    if (connected == ARTIK_WEBSOCKET_CONNECTED)
        fprintf(stdout, "Websocket connected\n");
}

void websocket_receive_callback(void *user_data, void *result)
{
    char *buffer = (char *)result;
    if (buffer == NULL) {
        fprintf(stdout, "receive failed\n");
        return;
    }
    printf("received: %s\n", buffer);
    free(result);
}
////websocket

////cloud////
artik_error send_cloud_message( const char *msg )
{
	artik_cloud_module *cloud = (artik_cloud_module *)artik_request_api_module("cloud");
	artik_error ret = S_OK;
	char *response = NULL;

	fprintf(stdout, "TEST: %s starting\n", __func__);
	artik_ssl_config ssl_config = {0};
	ret = cloud->send_message(token, device_id, msg, &response, &ssl_config);
	if (response) {
		fprintf(stdout, "TEST: %s response data: %s\n", __func__,response);
		free(response);
	}
	if (ret != S_OK) {
		fprintf(stdout, "TEST: %s failed (err=%d)\n", __func__, ret);
		return ret;
	}

	fprintf(stdout, "TEST: %s succeeded\n", __func__);
	artik_release_api_module(cloud);
	return ret;
}


