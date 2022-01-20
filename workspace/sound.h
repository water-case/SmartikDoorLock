#ifndef SOUND_H_
#define SOUND_H_

#include "main.h"

#define CHECK_RET(x)	{ if (x != S_OK) goto exit; }

void on_finished(void *userdata);
artik_error Sound(char* sound_filename);


#endif /* SOUND_H_ */
