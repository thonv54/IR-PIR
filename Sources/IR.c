/*
 * IR.c
 *
 *  Created on: Oct 30, 2015
 *      Author: hp8460p
 */

#include "Device.h"
#include "IR.h"
#include "FTM.h"
#include "Init_IO.h"
#include "SPI.h"
#include "UART.h"
#include "MTIM.h"
#include "KBI.h"
#include "RTC.h"
#include "CC.h"
#include "wdog.h"
#include "EEP_ROM.h"
#include "stdio.h"

#ifdef IR
//unsigned int TestData[10] = {9000,4500,500,500,500,500,500,500,500,500};
//unsigned char m_ucSPI0_TxBuff[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
//unsigned char m_ucSPI0_RxBuff[16];
//unsigned char FlashID[4]= {0};

unsigned char GetIR_flag = 0;

unsigned long IrDataLength = 0;
unsigned char IrCmdDataFlag = 0;
unsigned char IR_event = WaitCCCmd;
unsigned char ChangeStateEvent = 0;
irHeader_t NewIrHeader;
irHeader_t OldIrHeader;
char tempstr[100];

volatile unsigned char Mtim_cnt = 0;

extern unsigned char CC_ready_flag;

unsigned char WaitHighPull(void) {
	while (IR_IN == 0) {
		if (FTM2_CNT > 60000) {
			return 0;
		}
	}
	return 1;
}
unsigned char WaitLowPull(void) {
	while (IR_IN == 1) {
		if (FTM2_CNT > 60000) {
			return 0;
		}
	}
	return 1;
}
void CopyIrDataToCharArray(unsigned int *IntData, unsigned char *CharData) {
	unsigned char i;
	for (i = 0; i < 8; i++) {
		if ((i & 0x01) == 0) {
			CharData[i] = (IntData[i >> 1] >> 8) & 0xFF;
		} else {
			CharData[i] = IntData[i >> 1] & 0xFF;
		}
	}
}
void uint2uchar(unsigned int IntNumber, unsigned char *CharNumber) {
	CharNumber[0] = (IntNumber >> 8) & 0xFF;
	CharNumber[1] = IntNumber & 0xFF;

}
void uchar2uint(unsigned char *CharNumber, unsigned int IntNumber) {
	IntNumber = (CharNumber[0] << 8) | CharNumber[1];

}

unsigned char CheckIrDataStore(irHeader_t tempIrHeader) {
	unsigned int i;
	unsigned int tempFlashData;
	unsigned long irTotalTime = 0;

	if (tempIrHeader.cmdLen > 5000) {
		flash_sector_erase(tempIrHeader.no * ByteInPage);
		return 0;
	}
	if (tempIrHeader.no > MAX_FLASH_SECTOR) {
		flash_sector_erase(tempIrHeader.no * ByteInPage);
		return 0;
	}
	{
		for (i = 0; i < tempIrHeader.cmdLen; i++) {
			flash_read(
					tempIrHeader.no * ByteInPage + sizeof(tempIrHeader) + 2 * i,
					&tempFlashData, sizeof(tempFlashData));
			irTotalTime += tempFlashData;
#ifdef Debug
			(void) sprintf(tempstr, "%4u %4u\n\r", i, tempFlashData);
			UART0_puts(tempstr);
			__delay_ms(30);

#endif
		}
#ifdef Debug
//			UART0_puts("\n\r");
//			(void)sprintf(tempstr, "total%4u\n\r",(unsigned int)(irTotalTime/1000));
//			UART0_puts(tempstr);
#endif
		if (irTotalTime > 10000000UL) {
			flash_sector_erase(tempIrHeader.no * ByteInPage);
			return 0;
		}
	}
	return 1;
}

void IrSendCmd(unsigned long flash_add, unsigned long length) {
	unsigned int i;
	unsigned int IrData;
	for (i = 0; i < length; i++) {
		ResetTimer();
		if (i & 0x0001) {
			IR_OFF
			;
		} else {
			IR_ON;
		}
		flash_read(flash_add + 2 * i, &IrData, 2);
//		if (i & 0x0001) {
//			IrData += 54;
//		} else {
//			IrData += 57;
//		}
		while (FTM2_CNT < IrData - 4)
			;
	}
	IR_OFF
	;
}

void IR_Program(void) {
	switch (IR_event) {
	case LearnIRCmd:
		if (ChangeStateEvent == 1) {
			StatusLedOn
			// neu co trigger reset timer,
			ResetRTCTimer();
			ChangeStateEvent = 0;
			CC_D3_SendIrStatus(WaitIRStudy, NewIrHeader);
			EnableGetIRCommand
			;
		}
		// sau do kiem tra CC de biet ban tin dung hoc
		CC_receiver_program();
		//dong thoi cho phep lang nghe lenh ir 
		// dia chi de luu lenh IR trong ngat se la NewIrHeader.no 
		// neu da luu xong lenh IR
		if (IrCmdDataFlag) {
			IrCmdDataFlag = 0;

			// luu lai header vao flash va xoa bien header tam
			flash_page_program(NewIrHeader.no * ByteInPage, &NewIrHeader,
					sizeof(NewIrHeader));
//			for(i=0;i<=NewIrHeader.no;i++){
//				flash_read(i * ByteInPage, &OldIrHeader, sizeof(OldIrHeader));
//					UART0_puti(OldIrHeader.no);
//					UART0_puts("  ");
//					UART0_puti(OldIrHeader.remoteID);
//					UART0_puts("  ");
//					UART0_puti(OldIrHeader.remoteType);
//					UART0_puts("  ");
//					UART0_puti(OldIrHeader.cmdRemotePos);
//					UART0_puts("\n\r");
//			}
			StatusLedBlinkLongtime
			;
			// thoat ra khoi qua trinh hoc va gui lenh hoc xong toi cc
			DisableGetIRCommand
			;
			// gui thong bao trang thai toi cc
			CC_D3_SendIrStatus(IRStudyDone, NewIrHeader);
			NewIrHeader.no = 0xFFFF;
			IR_event = WaitCCCmd;
			StatusLedOff

		}
		if (RTC_CNT > 600) {
			IR_event = LearnTimeout;
		}
		break;
	case LearnTimeout:
		// neu qua timeout la 60s hoac co lenh stop
		// xoa header da luu va gui lenh qua timeout toi cc
		CC_D3_SendIrStatus(WaitIRStudyTimeOut, NewIrHeader);
		flash_sector_erase(NewIrHeader.no * ByteInPage);
		NewIrHeader.no = 0xFFFF;
		DisableGetIRCommand
		;
		IR_event = WaitCCCmd;
		StatusLedOff
		break;
	case WaitCCCmd:

//		if (ChangeStateEvent == 1) {
//			ChangeStateEvent = 0;
//		}
		CC_receiver_program();
		break;
	default:
		break;
	}
}

//void testflash(void){
////	irHeader_t tempNewIrHeader;
////	irHeader_t tempOldIrHeader;
//	unsigned char i;
//	NewIrHeader.no = 1;
//	NewIrHeader.remoteID = 2;
//	NewIrHeader.remoteType = 3;
//	NewIrHeader.cmdLen = 224;
//	for(i = 0; i<4; i++){
//		flash_sector_erase(i*ByteInPage);
//		flash_page_program(i*ByteInPage, &NewIrHeader, sizeof(NewIrHeader));
//	}
//	for(i=0;i<4;i++){
//		flash_read(i * ByteInPage, &OldIrHeader, sizeof(OldIrHeader));
//			UART0_puti(OldIrHeader.no);
//			UART0_puts("  ");
//			UART0_puti(OldIrHeader.remoteID);
//			UART0_puts("  ");
//			UART0_puti(OldIrHeader.remoteType);
//			UART0_puts("  ");
//			UART0_puti(OldIrHeader.cmdRemotePos);
//			UART0_puts("\n\r");
//	}
//}

void EraserFlashFirst(void) {
	unsigned int LoadProgram;
	LoadProgram = EEPROM_Read_int(0x3100);
	if (LoadProgram != OldLoad) {
		LoadProgram = OldLoad;
		EEPROM_Write_int(0x3100, LoadProgram);
		flash_erase();
		StatusLedBlinkLongtime
		;
	}
}
unsigned char CheckFlashConection(void) {
	unsigned char TempID[4] = { 0 };
	unsigned int Temp;
	flash_read_id(TempID);
	Temp = TempID[1] << 8 | TempID[2];
	if (Temp == 0x4015) {
		return 1;
	}
	return 0;
}
void IrStartUp(void) {
	unsigned char CheckCCReadyCnt = 0;
	//Kiem tra Flash
	if (CheckFlashConection() == 1) {
		RedLedOn;
		BlueLedOn;
	} else {
		RedLedBlinkLongTime
		;
		Reset_MCU();
	}
	//Kiem tra ket noi toi CC

#ifndef Debug
	while (CC_ready_flag == 0) {
		CheckCCReadyCnt++;
		CC_send_SynClock();
		__delay_ms(100);
		CC_receiver_program();
		if (CheckCCReadyCnt == 20) {
			RedLedBlinkLongTime
			;
			RedLedBlinkLongTime
			;
			Reset_MCU();
		}
	}
#endif
	// co cho phep hoc lenh = 0 ->khong cho phep hoc lenh,
	DisableGetIRCommand
	;
	// bien luu lenh hoc moi = 0xFFFF,  the khong co lenh hoc moi
	NewIrHeader.no = 0xFFFF;
	// trang thai ban dau la cho lenh
	IR_event = WaitCCCmd;
	// xoa falsh trong lan dau nap chip
#ifndef Debug
	EraserFlashFirst();
#endif
}
interrupt VectorNumber_Vmtim0 void MTIMO_ISR(void) {
	if (MTIM0_SC_TOF) {
		MTIM0_SC_TOF = 0;

		if (Mtim_cnt < _MTIM0_PWM) {
			IR_OUT = 1;
		} else {
			IR_OUT = 0;
		}
		Mtim_cnt++;
		if (Mtim_cnt == _MTIM0_PRESSCALE) {
			Mtim_cnt = 0;
		}
	}
}

interrupt VectorNumber_Vkbi0 void KBI0_ISR(void) {
	unsigned int GetIRCommand_step = 0;
//	unsigned long WriteFlashCnt = 0;
	unsigned int IrIntData = 0;
	unsigned char waitStatus = 0;
	unsigned char IrCharData[2] = { 0 };
	KBI0_SC_KBIE = 0;

	//neu co co cho phep hoc lenh
	if (GetIR_flag == 1) {
		// neu IR header co ton tai moi cho phep hoc lenh
		if (NewIrHeader.no != 0xFFFF) {

			ResetTimer();
			//		GetIR_flag = WaitHighPull();
			while (GetIR_flag == 1) {

				// neu step %2 = 0
				if (GetIRCommand_step % 2 == 0) {
					//doi suon len
					waitStatus = WaitHighPull();
				}
				// neu khong 
				else {
					//doi suon xuong
					waitStatus = WaitLowPull();
				}
				//sau do
				//neu thoi gian doi lon hon 60000
				if (waitStatus == 0) {
					//-> ket thuc
					GetIR_flag = 0;
					if (GetIRCommand_step > 10) {				// lenh IR it nhat phai lon hon 1 byte
						NewIrHeader.cmdLen = GetIRCommand_step;
						IrCmdDataFlag = 1;
					} else {
						IrCmdDataFlag = 0;
					}
				}
				// neu khong 
				else {
					//- > luu data
					IrIntData = FTM2_CNT;
					// reset timer
					ResetTimer();

					IrCharData[0] = (IrIntData >> 8) & 0xFF;
					IrCharData[1] = IrIntData & 0xFF;

					flash_page_program(
							(NewIrHeader.no * ByteInPage)
									+ (GetIRCommand_step * 2)
									+ sizeof(NewIrHeader), IrCharData, 2);
					// bien+ 1
					GetIRCommand_step++;
				}
			}
		}
	}
	KBI0_SC_KBIE = 1;
	KBI0_SC_KBACK = 1;
}

#endif
