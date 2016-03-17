/*
 * MTMI.c
 *
 *  Created on: Oct 2, 2014
 *      Author: hp8460p
 */

#include "MTIM.h"


/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: MTIM0_Init
   *
   * @brief initialize MTIM0
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
void Init_MTIM0( void )
{
#ifdef IR
	MTIM0_CLK_CLKS = BUS_CLOCK;   // fixed clock
	MTIM0_CLK_PS = 0b0001;     // prescale is 256
	MTIM0_MOD	= 87;		//  module couter mode					
	MTIM0_SC_TRST = 1;		// RST count
	MTIM0_SC_TOIE = 1;      // enable TOF interrupt  // 0 disabled
	MTIM0_SC_TOF  = 0;      // ensure interrupt flag =0
	MTIM0_SC_TSTP = 0;      // count active
	//	Fmtmi = 20mhz/256 = 80khz
	//  Fmtmi = 10mhz/125 = 80khz
	//			10mhz/135 = 74khz
	//  Fmtim = 10mhz/131    = 76khz
	//	Fmtim				= 87 -> 38khz neu chia 3
	// Fmtim	=			= 65 -> 38khz neu chia 4
#endif
#ifdef PIR
	MTIM0_CLK_CLKS = BUS_CLOCK;   // fixed clock
	MTIM0_CLK_PS = 0b1111;     // prescale is 256
	MTIM0_MOD	= 255;		//  module couter mode					
	MTIM0_SC_TRST = 1;		// RST count
	MTIM0_SC_TOIE = 1;      // enable TOF interrupt  // 0 disabled
	MTIM0_SC_TOF  = 0;      // ensure interrupt flag =0
	MTIM0_SC_TSTP = 0;      // count active
	//	Fmtmi = 20mhz/256 = 80khz
	//  Fmtmi = 10mhz/125 = 80khz
	//			10mhz/135 = 74khz
	//  Fmtim = 10mhz/131    = 76khz
	//	Fmtim				= 87 -> 38khz neu chia 3
	// Fmtim	=			= 65 -> 38khz neu chia 4
#endif
}	













