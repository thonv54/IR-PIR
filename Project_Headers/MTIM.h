/*
 * MTIM.h
 *
 *  Created on: Oct 2, 2014
 *      Author: hp8460p
 */


#ifndef MTIM_H_
#define MTIM_H_

#include "Device.h"


#define __38Khz_Pres3

#define BUS_CLOCK		0b00
#define FIXER_CLOCK		0b01
#define EXT_R_CLOCK		0b10
#define EXT_F_CLOCK		0b11

#define IR_OFF	{(MTIM0_SC_TSTP = 1); (IR_OUT = 0);}
#define IR_ON	(MTIM0_SC_TSTP = 0)


#ifdef __38Khz_Pres4
#define _MTIM0_MOD_VALUE		65
#define _MTIM0_PRESSCALE		4
#define _MTIM0_PWM			3

#endif

#ifdef __38Khz_Pres3
#define _MTIM0_MOD_VALUE		87
#define _MTIM0_PRESSCALE		3
#define _MTIM0_PWM			1
#endif	

#ifdef __40Khz
#define _MTIM0_MOD_VALUE		125
#define _MTIM0_PRESSCALE		2
#define _MTIM0_PWM			1
#endif



void Init_MTIM0( void );


#endif /* MTIM_H_ */
