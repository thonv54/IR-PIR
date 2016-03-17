/*
 * FTM.c
 *
 *  Created on: Oct 25, 2014
 *      Author: hp8460p
 */
#include "FTM.h"
#include "Device.h"
void Init_PWM(void) {
	FTM2_MODE_WPDIS = 1;
	FTM2_MODE_FTMEN = 1;
	FTM2_CNTIN = 1;
	FTM2_COMBINE2 = 0x03;
	FTM2_C4V = 30000;
	FTM2_CNT = 0;
	FTM2_MOD = 60000;

	FTM2_C4SC = 0x28;

	FTM2_SC_PS = 0b011;

	FTM2_SC_CLKS = 0b00;
}
void Init_FLEX_TIMER(void) {
							// FTM2_SC_PS = 100 Up Counting mode  1Cnt = 0.8uS
							// FTM2_SC_PS = 101 Up Counting mode  1Cnt = 1.6uS
							// FTM2_SC_PS = 110 Up Counting mode  1Cnt = 3.2uS
							// FTM2_SC_PS = 111 Up Counting mode  1Cnt = 6.4uS	
	FTM2_MODE_FTMEN = 1;
	FTM2_CNTIN = 0;
	FTM2_CNT = 0;
	FTM2_MOD = 0xFFFF;				
	FTM2_SC_TOIE = 0;
	FTM2_SC_CPWMS = 0;		
#ifdef IR
	FTM2_SC_PS = 0b101;
#endif
#ifdef PIR
	FTM2_SC_PS = 0b111;
#endif
	FTM2_SC_CLKS = 0b01;
}
void ResetTimer(void){
	FTM2_SC_CLKS = 0;
	FTM2_CNT = 0;
	FTM2_SC_CLKS = 1;
}
void Init_FTM0(void){
		// FTM2_SC_PS = 100 Up Counting mode  1Cnt = 0.8uS
		// FTM2_SC_PS = 101 Up Counting mode  1Cnt = 1.6uS
		// FTM2_SC_PS = 110 Up Counting mode  1Cnt = 3.2uS
		// FTM2_SC_PS = 110 Up Counting mode  1Cnt = 6.4uS
		// FTM2_SC_PS = 111 Up Counting mode  1Cnt = 12.8uS	
	FTM0_CNT = 0;
	FTM0_MOD = 39062;					// 390625 = 0.5s				
	FTM0_SC_TOIE = 1;
	FTM0_SC_CPWMS = 0;		
	FTM0_SC_PS = 0b111;
	FTM0_SC_CLKS = 0b01;
}
void disable_FTM0(void) {
	FTM0_SC_CLKS = 0;

}
void reset_FTM0(void) {
	FTM0_SC_CLKS = 0;
	FTM0_CNT = 0;
}
