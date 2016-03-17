/*
 * FTM.h
 *
 *  Created on: Oct 25, 2014
 *      Author: hp8460p
 */


#ifndef FTM_H_
#define FTM_H_

#define _FTM_10MS		1562
#define _FTM_100MS		15625
#define _FTM_5MS		781
#define _FTM_100uS		15
#define _FTM_500uS		78
#define _FTM_1MS		156
#define _FTM_20MS		3125
#define _FTM_200MS		31250

#define ResetFTMTimer ResetTimer	

void Init_FLEX_TIMER(void);
void Init_PWM(void);
void disable_FTM0(void);
void reset_FTM0(void);
void ResetTimer(void);
void Init_FTM0(void);

#endif /* FTM_H_ */
