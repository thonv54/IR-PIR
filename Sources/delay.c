/*
 * delay.c
 *
 *  Created on: Oct 1, 2014
 *      Author: hp8460p
 */

#include "delay.h"

void __delay_ms(unsigned int n){
	unsigned int i;
	for(i=0;i<n;i++){
		__delay_us(750);
	}
}
void __delay_s(unsigned int n){
	unsigned int i;
	for(i=0;i<n;i++){
		__delay_ms(1000);
	}
}
void __delay_us(unsigned int  n){
	unsigned int i;
	for(i = 0; i < n; i++)
	{
			asm(nop);
	}
}
void __delay_x100us(unsigned int n){
	unsigned int i = 0;
	for(i = 0; i< n; i++)
	{
		__delay_x5us(20);
	}
}
void __delay_x5us(unsigned int n){
	unsigned int i;
	for(i = 0; i < n; i++)
	{
//		1
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		
//2
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
//	3	

		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
//	4	

		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
//5
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
//6
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
//	7	
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
//	8	

		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		asm(nop);
		
	}
}

unsigned int abs2int(unsigned int a, unsigned int b) {
	unsigned int temp = 0;
	if (a > b) {
		temp = a - b;
	} else {
		temp = b - a;
	}
	return temp;
}

unsigned int max(unsigned int a, unsigned int b){
	if(a>b){
		return a;
	}
	return b;
}
