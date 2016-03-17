/*
 * CC.h
 *
 *  Created on: Nov 1, 2014
 *      Author: hp8460p
 */

#include "Device.h"

#ifndef CC_H_
#define CC_H_


unsigned char xor_str(unsigned char *s, unsigned int length);
unsigned char xor_int(unsigned int intnumber);

void CC_receiver_program(void);
void CC_send_SynClock(void);
void CC_send_ReadyMC(void);
void CC_D3_SendIrStatus(unsigned char IR_State, irHeader_t tempIrHeader);
void CC_D5_SendIrCmdInfo(unsigned int IR_no);
void CC_D7_SendIrTotalCmd(void);
void CC_DA_SendMaxIdRemote(void);
void CC_C3_LearnIRCmd(unsigned char *s);
void CC_C4_EraserIRCmd(unsigned char *s);
void CC_C5_ReadIRCmd(unsigned char *s);
void CC_C6_SetIRCmd(unsigned char *s);
void CC_C7_ReadIRCmdNumber(void);
void CC_C8_StopLearnIRCmd(void);
void CC_C9_AddIrCmdInfo(unsigned char *s);
void CC_CA_FindMaxIdRemote(void);
void CC_BE_SyncTime(unsigned char *s);
void CC_BD_MCSendVertion(void);

void CC_F0_PirSendData(void);
void CC_E0_CCGetPirStatus(void);
void CC_E1_CCSendDataDone(void);
#endif /* CC_H_ */
