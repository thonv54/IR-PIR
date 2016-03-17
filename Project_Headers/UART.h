/*
 * UART.h
 *
 *  Created on: Oct 1, 2014
 *      Author: hp8460p
 */




#ifndef UART_H_
#define UART_H_

void Init_UART0(unsigned int BaudRate);

void UART0_putchar(unsigned char send);
unsigned char UARTO_getchar(void);
void UART0_puts( char *s);
void UART0_putnum(unsigned int num);
void UART0_SendBurst(char *s, char length);
void UART_debug_TSI(void);
void TurnOffUART(void);
void TurnOnUART(void);
void i2s(int i,char *s);// Convert Integer to String
int s2i(char *s);	// Convert String to Integer
void UART0_puti(int i);	// Put Integer to UART, Require i2s(),uart_puts()
void UART0_putstr(unsigned char *s, unsigned int length);
#endif /* UART_H_ */
