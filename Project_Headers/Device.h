/*
 * Device.h
 *
 *  Created on: Oct 29, 2015
 *      Author: hp8460p
 */

#ifndef DEVICE_H_
#define DEVICE_H_



#define SendCCKeyID	UART0_putchar(0x4C);	UART0_putchar(0x4D)

#define CodeVer		2

#include "mc9s08pt16.h"
#include "derivative.h"
#include <hidef.h> /* for EnableInterrupts macro */
#include "delay.h"


#define IR_DEBUG
#undef IR
#undef PIR_CC

//#define PIR_No_CC
//#undef IR
//#undef IR_DEBUG
//#undef PIR_CC
//
//#define IR
//#undef PIR_CC
//#undef PIR_No_CC

#ifdef PIR_No_CC
#define PIR
#undef CC_Interface

#define CodeType	0x16
#define CodeOpption 0x01
#endif

#ifdef PIR_CC
#define PIR
#define CC_Interface
#define CodeType	0x16
#define CodeOpption 0x02
#endif

#ifdef IR_DEBUG
#define IR
#undef PIR_CC
#undef PIR_No_CC
#define Debug
#endif

#ifdef IR
#define IR
#define CodeType	0x17
#define CodeOpption 0x00
#endif



#define ByteInPage 4096UL
#define MAX_FLASH_SECTOR 511
#define TempFlashStore	(MAX_FLASH_SECTOR - 1)
#define MAX_REMOTE_TYPE 10
#define IR_NAME_LEN 32
#define On			1
#define Off			0

	
typedef struct{
	unsigned int type;
	unsigned int total;
}cmdType_t;

typedef struct {
	unsigned int no;
	unsigned int remoteID;
	unsigned int remoteType;
	unsigned int cmdRemotePos;
	unsigned int cmdLen;
	unsigned char cmdName[IR_NAME_LEN];
	unsigned char cmdInfo[4];
} irHeader_t;

typedef enum{
	airc	=	0x01,
	tv		= 	0x02,
	fan		=	0x03,
}RemoteTypeEnum;

typedef enum{
	WaitIRStudy			=	0x01,
	WaitIRStudyTimeOut	=	0x02,
	IRStudyDone			=	0x03,
	MemoryFull			=	0x04,
	GetExtraInforAndStoreDone	=	0x05,
	GetExtraInforAndStoreError	=	0x06,
	RunCmdDone			=	0x07,
	ErraseCmdDone		= 	0x08,
	CmdNotHave			= 	0x09,
	
}CCIrStatusEnum;

typedef enum{
	WaitIrData = 1,
	IrInterrupt = 2,
	IrStoreEnd = 3,
}ProgramEventEnum;

typedef enum{
	WaitCCCmd	=	1,
	LearnIRCmd	=	2,
	LearnTimeout =  3,
	
}IR_eventEnum;

typedef struct{
	unsigned char DeviceType;
	unsigned char Channel;
	unsigned int PanID;
	unsigned int NwkAddress;
	unsigned char KeyNwk[8];
}PirZigbeeNwk_t;

typedef enum{
	NwkDone	=	1,
	NwkEmpty =	0,
}PirNwkStatusEnum;

typedef struct{
	unsigned char 	PIRStatus;
	unsigned int 	LightSensor;
	unsigned int 	LightLevel;
	unsigned int 	TimeDelay;
}PirStatus_t;

typedef enum {
	Pir_on	=	2,
	Pir_off =	1,
}PirSttEnum;
#endif /* DEVICE_H_ */
