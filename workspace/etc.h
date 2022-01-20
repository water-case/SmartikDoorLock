#ifndef ETC_H_
#define ETC_H_

#include "main.h"


void interupt_button();
void flash(int num, int n);
void originflash(int num, int n);
void flashon();
void flashoff();
void OpenAndClose();//short motor_pin);
void Close();
bool MADC();
unsigned char getAbit(unsigned char x, unsigned char n);

#endif /* ETC_H_ */
