/*
 * wdog.c
 *
 *  Created on: Jul 11, 2014
 *      Author: Dang Giap
 */


#include "wdog.h"
#include "Device.h"
#include "delay.h"

void wdog_disable(void) {
	wdog_unlock();
	/* Write all 6 registers once within 128 bus cycles after unlocking */
	WDOG_CS1 = 0x20;
	WDOG_CS2 = 0;
	WDOG_TOVALH = 0xFF;
	WDOG_TOVALL = 0xFF;
	WDOG_WINH = 0;
	WDOG_WINL = 0;
}

void wdog_enable(void) {
	wdog_unlock();

	/* NOTE: the following write sequence must be completed within 128 buc clocks
	 * 
	 */
	/* enable watchdog */

	WDOG_CS1 = 0x8B | WDOG_CS1_UPDATE_MASK;
	//  | WDOG_CS1_WDOGSE_MASK 
	//  | WDOG_CS1_WDOGWE_MASK ;		// debug enable
	WDOG_CS2 = 1; /* use internal 1K clock as clock source */

	WDOG_TOVAL = 0x80FF;
}

void wdog_refreshing(void)
{
	DisableInterrupts; // disable global interrupt
	WDOG_CNT = 0xA602; // write the 1st refresh word
	WDOG_CNT = 0xB480; // write the 2nd refresh word to refresh counter
	EnableInterrupts; // enable global interrupt
}

//t_timeout=time_out/clock_source*1000 (with clock source is 1k or 32kHz
//clock_source=0-3 (0: Bus clock; 1: 1kHz; 2: 32kHz; 3: external clock)
void wdog_reconfiguring(unsigned int time_out, unsigned char clock_source)
{
	DisableInterrupts; // disable global interrupt
	WDOG_CNT = 0xC520; // write the 1st unlock word
	WDOG_CNT = 0xD928; // write the 2nd unlock word
	WDOG_TOVAL = time_out; // setting timeout value
	WDOG_CS2 = clock_source; // setting clock source
	WDOG_CS1 = WDOG_CS1_EN_MASK; // enable counter running
	EnableInterrupts; // enable global interrupt
}
void Reset_MCU(void){
	DisableInterrupts; // disable global interrupt
	WDOG_CNT = 0xC520; // write the 1st unlock word
	WDOG_CNT = 0xD928; // write the 2nd unlock word
	WDOG_TOVAL = 2; // setting timeout value
	WDOG_CS2 = 1; // setting clock source
	WDOG_CS1 = WDOG_CS1_EN_MASK; // enable counter running
	EnableInterrupts; // enable global interrupt
	__delay_ms(1000);
}


