/*
 * UART.c
 *
 *  Created on: Oct 1, 2014
 *      Author: hp8460p
 */

#include "UART.h"
#include "Device.h"

//#define Bus_clock 10000000UL
#define Bus_clock 20000000UL

void Init_UART0(unsigned int BaudRate) {

	SCI0_BD = (unsigned char) ((SCI0_BD & 0xE000)
			| ((Bus_clock >> 4) / BaudRate));
	SCI0_C1 = 0;     // 8bit mode, 1 stop bit, no parity
	SCI0_C2_RIE = 1;	// enable receiver interrupt
	SCI0_C2_TE = 1;  // enable TX
	SCI0_C2_RE = 1;	// disable RX

}	//end Init_SCI0_
void TurnOffUART(void) {
	SCI0_C2_RIE = 0;	// enable receiver interrupt
	SCI0_C2_TE = 0;  // enable TX
	SCI0_C2_RE = 0;	// disable RX
}
void TurnOnUART(void) {
	SCI0_C2_RIE = 1;	// enable receiver interrupt
	SCI0_C2_TE = 1;  // enable TX
	SCI0_C2_RE = 1;	// disable RX	
}
void UART0_putchar(unsigned char send) {
	char dummy;
	while ((SCI0_S1 & SCI0_S1_TDRE_MASK) != SCI0_S1_TDRE_MASK)
		;
	dummy = SCI1_S1;
	SCI0_D = send;
//	__delay_ms(50);

}
unsigned char UART0_getchar(void) {
	unsigned char dummy;
	while ((SCI0_S1 & SCI0_S1_RDRF_MASK) != SCI0_S1_RDRF_MASK)
		;

	dummy = SCI0_S1;
	dummy = SCI0_D;

	return dummy;

}

void UART0_puts(char *s) {
	while (*s) {
		UART0_putchar(*s++);
	}
}
void UART0_putstr(unsigned char *s, unsigned int length){
	unsigned int i;
	for(i=0;i<length;i++){
		UART0_putchar(s[i]);
	}
}
void UART0_SendBurst(char *s, char length) {
	char i;
	for (i = 0; i < length; i++) {
		UART0_putchar(*s++);
	}

}

//void UART0_putnum(unsigned int num) {
//	char buff[10];
//	sprintf(buff, "%4u", num);
//	UART0_puts(buff);
//}
void ui2s(unsigned int i, char *s)	// Convert Integer to String
{
	short len;
	char *p;
	len = 0;
	p = s;	
	do {
		*s = (i % 10) + '0';
		s++;
		len++;
		i /= 10;
	} while (i != 0);
	for (i = 0; i < len / 2; i++) {
		p[len] = p[i];
		p[i] = p[len - 1 - i];
		p[len - 1 - i] = p[len];
	}
	p[len] = 0;
}
int s2i(char *s)	// Convert String to Integer
{
	int i = 0;
	char sign = '+';
	if (*s == '-') {
		sign = '-';
		s++;
	}
	while (*s != 0) {
		i = i * 10 + (*s - '0');
		s++;
	}
	if (sign == '-')
		i = -i;
	return i;
}
void UART0_puti(int i)	// Put Integer to UART, Require i2s(),uart_puts()
{
	char s[7];
	ui2s(i, s);
	UART0_puts(s);
}
