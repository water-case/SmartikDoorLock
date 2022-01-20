#include "sound.h"




void on_finished(void *userdata)
{


	fprintf(stdout, "TEST: finished playing sound\n");

	//sloop->quit();
}

artik_error Sound(char* sound_filename)
{
	artik_error ret = S_OK;
	artik_media_module *media = (artik_media_module *)artik_request_api_module("media");


	ret = media->play_sound_file(sound_filename);
	//if (ret != S_OK)
		//goto exit;

	//ret = media->set_finished_callback(on_finished, (void*)sloop);
	//if (ret != S_OK)
		//goto exit;
/*
	sloop->run();
*/

exit:
	fprintf(stdout, "TEST: %s %s\n", __func__, (ret == S_OK) ? "succeeded" : "failed");

	//artik_release_api_module(media);
	//artik_release_api_module(sloop);

	return ret;
}
