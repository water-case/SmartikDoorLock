#include "sound.h"

void on_finished(void *userdata) {
	fprintf(stdout, "TEST: finished playing sound\n");
}

artik_error Sound(char* sound_filename) {
	artik_error ret = S_OK;
	artik_media_module *media = (artik_media_module *)artik_request_api_module("media");

	ret = media->play_sound_file(sound_filename);

exit:
	fprintf(stdout, "TEST: %s %s\n", __func__, (ret == S_OK) ? "succeeded" : "failed");
	return ret;
}
