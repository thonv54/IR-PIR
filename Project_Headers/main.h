



#ifndef MAIN_H_
#define MAIN_H_

#include "Device.h"
#include "Init_IO.h"
#include "TSI.h"
#include "RTC.h"
#include "UART.h"
#include "delay.h"
#include "EEP_ROM.h"
#include "common.h"
#include "MTIM.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "ADC.h"
#include "FTM.h"
#include "CC.h"
#include "wdog.h"
#include "Fan.h"


void button_detect(void);
void TSI_scan_chanel(unsigned char chanel);
void button_scan(void);
void store_opption(void);
//void update_base(void);
void ButtonScanChanel(unsigned char i);
void backup(void);

#endif /* MAIN_H_ */
