#ifndef MAIN_H_
#define MAIN_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

#include <artik_module.h>
#include <artik_loop.h>
#include <artik_platform.h>
#include <artik_gpio.h>
#include <artik_time.h>
#include <artik_bluetooth.h>
#include <artik_adc.h>
#include <artik_pwm.h>
#include <artik_mqtt.h>
#include <artik_cloud.h>
#include <artik_media.h>

#include "socket.h"
#include "settinginitial.h"
#include "keypad.h"
#include "secret.h"
#include "loop.h"
#include "allsend.h"
#include "sound.h"
#include "etc.h"

extern artik_gpio_module *gpio;
extern artik_loop_module *loop;
extern artik_time_module *timer;
extern artik_pwm_module *pwm;
extern artik_adc_module *adc;
extern artik_bluetooth_module *bt;
extern artik_cloud_module *cl;


extern struct boolvariable boolvariable;
extern struct charvariable charvariable;
extern struct intvariable intvariable;
extern struct artikvariable artikvariable;


#endif /* MAIN_H_ */
