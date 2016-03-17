/*
 * KBI.c
 *
 *  Created on: Oct 29, 2014
 *      Author: hp8460p
 */


#include "Device.h"
#include "KBI.h"


void Init_KBI(void){
//	PORT_PTAPE = PORT_PTAPE |PORT_PTAPE_PTAPE1_MASK; // Ensure use Pull up resister
#ifdef IR
	KBI0_SC_KBIE = 0;
	KBI0_SC_KBF = 0;
	KBI0_SC_KBACK = 1;
	KBI0_SC_KBMOD = 0;
	KBI0_ES_KBEDG1 = 0;
	KBI0_PE_KBIPE1 = 1;
#endif
#ifdef PIR
	KBI0_SC_KBIE = 0;
	KBI0_SC_KBF = 0;
	KBI0_SC_KBACK = 1;
	KBI0_SC_KBMOD = 0;
	KBI0_ES_KBEDG0 = 0;
	KBI0_PE_KBIPE0 = 1;
	KBI0_SC_KBIE = 1;
#endif
}
 
