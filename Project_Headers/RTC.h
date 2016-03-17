/*
 * RTC.h
 *
 *  Created on: Oct 1, 2014
 *      Author: hp8460p
 */




#ifndef RTC_H_
#define RTC_H_
#include "Device.h"

#define EXT_CLK		0b00
#define REAL_CLK	0b01

#define OFF_RTC		0b000
#define _128MS		1
#define _256MS		2
#define _512MS		3
#define _1024MS		4
#define _2048MS		5
#define _100MS		6
#define _1S			7

#define RTC_1S		10
#define RTC_2S		20
#define RTC_3S		30
#define RTC_3S5		35
#define RTC_5S		50
#define RTC_8S		80
#define RTC_10S		100
#define RTC_MAX		0xFFFF
#define RTC_12S		120
#define RTC_200MS	2
#define RTC_100MS	1

#define RTC_14S		140
#define RTC_16S		160
#define RTC_ERROR	180
#define DisableRTCTimer RTC_SC2_RTCPS = 0
#define EnableRTCTimer	RTC_SC2_RTCPS = _100MS


#define RTC_BUTTON_LV1		15
#define RTC_BUTTON_LV2		30
#define RTC_BUTTON_LV3		60
void Init_RTC(void);
void ResetRTCTimer(void);

#endif /* RTC_H_ */
