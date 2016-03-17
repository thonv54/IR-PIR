/*
 * wdog.h
 *
 *  Created on: Jul 11, 2014
 *      Author: 
 */


#ifndef WDOG_H_
#define WDOG_H_

#define wdog_unlock()	asm(LDA	#0xC5); asm(STA	0x3032); asm(LDA #0x20);asm(STA 0x3033);\
		asm(LDA	#0xD9); asm(STA	0x3032); asm(LDA #0x28); asm(STA 0x3033)

void wdog_disable(void);
void wdog_enable(void);
void wdog_refreshing(void);
void wdog_reconfiguring(unsigned int time_out, unsigned char clock_source);
void Reset_MCU(void);

#endif /* WDOG_H_ */
