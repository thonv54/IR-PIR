/*
 * RTC.c
 *
 *  Created on: Oct 1, 2014
 *      Author: hp8460p
 */

#include "RTC.h"
#include "Device.h"
void Init_RTC(void) {

	RTC_SC2_RTCPS = _100MS;		// T_rtc= PS_value/F_bus	64
	RTC_MOD = RTC_MAX;		// 640us
	RTC_SC1_RTIF = 0;
	RTC_SC1_RTIE = 0;
	RTC_SC2_RTCLKS = REAL_CLK;
}
void ResetRTCTimer(void){
	RTC_SC2_RTCPS  = 0;
	RTC_CNT = 0;
	RTC_SC2_RTCPS = _100MS;
}
void RTC_polling(void) {

	RTC_SC2_RTCPS = _100MS;		// T_rtc= PS_value/F_bus	64
	RTC_MOD = RTC_MAX;		// 640us
	RTC_SC1_RTIF = 0;
	RTC_SC1_RTIE = 0;
	RTC_SC2_RTCLKS = REAL_CLK;
}

