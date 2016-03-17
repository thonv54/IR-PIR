/*
 * RelayControl.c
 *
 *  Created on: Nov 13, 2015
 *      Author: hp8460p
 */

#include "Device.h"
#include "RelayControl.h"
#include "ADC.h"
#include "CC.h"
#include "Init_IO.h"

unsigned char PowerType = No_PW;
unsigned int RL_on_delay	=	50;
unsigned int RL_off_delay 	=	50;
extern unsigned char RlStatusFlag;

unsigned char OnStatusChangeFlag = 1;
unsigned char OffStatusChangeFlag = 0;

void GetPwSource(void) {
	unsigned int i = 0;
	unsigned char temp_status_pw;
	temp_status_pw = ZERO_DT_PW;
	while (i < 100){
		i++;
		if (ZERO_DT_PW != temp_status_pw) {
			PowerType = Have_PW;
			return ;
		}
		__delay_us(200);
	}
	PowerType = No_PW;
}
void WaitPWFallingEgle(void) {
	ADCWaitZeroPoint(ADC_ZERO_DT_PW);
}
void RL_on(void){
	if(OnStatusChangeFlag == 1){
		RedLedOn;
		if (PowerType == Have_PW) {
			WaitPWFallingEgle();
			__delay_x100us(100 - RL_on_delay);
		}
		Switch_on;	
		RlStatusFlag = On;	
		OnStatusChangeFlag = 0;
		OffStatusChangeFlag = 1;
	}
}
void RL_off(void){
	if(OffStatusChangeFlag == 1){
		RedLedOff;
		if (PowerType == Have_PW) {
			WaitPWFallingEgle();
			__delay_x100us(100 - RL_off_delay);
		}
		Switch_off;

		RlStatusFlag = Off;
		OffStatusChangeFlag = 0;
		OnStatusChangeFlag = 1;
	}
}
