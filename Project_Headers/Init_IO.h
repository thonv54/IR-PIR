/*
 * IO.h
 *
 *  Created on: Sep 30, 2014
 *      Author: hp8460p
 */
#include "Device.h"

#ifndef IO_H_
#define IO_H_
#define Buzz	PORT_PTAD_PTAD6

//#define TRIM_VALUE_39K0625HZ  0x50

#define BlueLedOn		PORT_PTDD_PTDD0	= 1
#define BlueLedOff		PORT_PTDD_PTDD0 = 0
#define RedLedOn		PORT_PTDD_PTDD1 = 1
#define RedLedOff		PORT_PTDD_PTDD1 = 0

#define AllLedOff		{BlueLedOff; RedLedOff;}
#define AllLedOn		{BlueLedOn; RedLedOn;}
#define AllLedBlink		{AllLedOn; __delay_ms(500); AllLedOff; __delay_ms(500);}
#define BuzzOn			{Buzz = 1;}
#define BuzzOff			{Buzz = 0;}

#define BlinkLedDetect	{AllLedOn; BuzzOn; __delay_ms(500); AllLedOff; BuzzOff; __delay_ms(500);}

#define RedLedBlinkLongTime {PORT_PTDD_PTDD1 ^=1 ; __delay_ms(200);PORT_PTDD_PTDD1 ^=1 ; __delay_ms(200);}
#define RedLedBlinkForDetect {PORT_PTDD_PTDD1 ^=1 ; __delay_ms(500);PORT_PTDD_PTDD1 ^=1 ; __delay_ms(500);}

#define StatusLedOn			BlueLedOn;
#define StatusLedOff		BlueLedOff;
#define StatusLedBlink		{PORT_PTDD_PTDD0 ^= 1; __delay_ms(50);  PORT_PTDD_PTDD0 ^= 1; __delay_ms(50);}
#define StatusLedBlinkLongtime {PORT_PTDD_PTDD0 ^= 1; __delay_ms(200);  PORT_PTDD_PTDD0 ^= 1; __delay_ms(200);}
#define StatusLedBlink500ms	{PORT_PTDD_PTDD0 ^= 1; __delay_ms(500);}


#define TurnOffCC	{PORT_PTCD_PTCD6 = 1;}
#define TurnOnCC	{PORT_PTCD_PTCD6 = 0;}
/*

* ----Cau hinh port A------		
* 
* 	PA0:		PIR_OUT			Output
*   PA1:		IR_IN			Input
*   PA2:		RX_MC			Input
*   PA3:		TX_MC			Output
* 	PA4: 		BKGD
* 	PA5: 		RESET
* 	PA6:		IR_OUT			Output
* 	PA7:		Not connect		Output
* 	
* ----Cau hinh port B------
* 
* 	PB0: 		ZERO_DT_PW		Input Analog	
* 	PB1:		Not connect		Output
* 	PB2:		SPI_CLK			Output
* 	PB3:		SPI_MOSI		Output
* 	PB4:		SPI_MISO		Input
* 	PB5:		SPI_CS			Output
* 	PB6:		SDA				Output
* 	PB7:		SCL				Output
* 
* 	----Cau hinh port C------
* 	
* 	PC0:		Not connect		Output 
* 	PC1:		VR_Light		Input Analog
* 	PC2:		VR_Timer		Input Analog
* 	PC3:		Not connect		Output	
* 	PC4:		Select			Output
* 	PC5:		RL1				Output
* 	PC6: 		CT_PW_CC		Output
* 	PC7:		BUTTON			Input
* 	
* 	----Cau hinh port D------
* 	
* 	PD0: 		Led_blue 			Output
* 	PD1:		Led_red 			Output 
* 	PD2:		Not Connect			Output
* 	PD3:		Not Connect			Output
* 	PD4: 
* 	PD5: 
* 	PD6: 
* 	PD7:	
*/




void Init_IO(void);
void clock_config(void);
void system_config(void);


#endif /* IO_H_ */
