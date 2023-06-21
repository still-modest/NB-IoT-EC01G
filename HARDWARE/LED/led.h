#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

void LED_Init(void);//≥ı ºªØ
#define LED1     PBout(0)
#define LED2  	 PCout(7)
#define RESET   PAout(8)

#endif
