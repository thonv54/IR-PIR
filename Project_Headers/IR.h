/*
 * IR.h
 *
 *  Created on: Oct 30, 2015
 *      Author: hp8460p
 */

#ifndef IR_H_
#define IR_H_

#include "Device.h"


#define 		IR_OUT			PORT_PTAD_PTAD6
#define 		IR_IN			PORT_PTAD_PTAD1

#define EnableGetIRCommand	{KBI0_SC_KBIE = 1; GetIR_flag = 1; }
#define DisableGetIRCommand {KBI0_SC_KBIE = 0; GetIR_flag = 0; }

#define OldLoad			0x1234

unsigned char WaitHighPull(void);
unsigned char WaitLowPull(void);
void uint2uchar(unsigned int IntNumber, unsigned char *CharNumber);
void uchar2uint(unsigned char *CharNumber, unsigned int IntNumber);
void CopyIrDataToCharArray(unsigned int *IntData, unsigned char *CharData); 
void GetIrCommand(void);
unsigned char CheckIrDataStore(irHeader_t tempIrHeader);
void IrSendCmd(unsigned long flash_add, unsigned long length);
void IR_Program(void);
void IrStartUp(void);
#endif /* IR_H_ */
