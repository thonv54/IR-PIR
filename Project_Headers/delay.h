/*
 * delay.h
 *
 *  Created on: Oct 1, 2014
 *      Author: hp8460p
 */

#ifndef DELAY_H_
#define DELAY_H_


void __delay_ms(unsigned int n);
void __delay_s(unsigned int n);
void __delay_us(unsigned int n);
void __delay_x5us(unsigned int n);
void __delay_x100us(unsigned int n);
unsigned int abs2int(unsigned int a, unsigned int b);
unsigned int max(unsigned int a, unsigned int b);
#endif /* DELAY_H_ */
