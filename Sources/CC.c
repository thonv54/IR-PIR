/*
 * CC.c
 *
 *  Created on: Nov 1, 2014
 *      Author: hp8460p
 */

#include "Device.h"
#include "CC.h"
#include "UART.h"
#include "EEP_ROM.h"
#include "delay.h"
#include "Init_IO.h"
#include "stdio.h"

#ifdef PIR
#include "PIR.h"
#include "I2C.h"
#include "ADC.h"
#endif
#ifdef IR
#include "IR.h"
#include "SPI.h"
#endif

unsigned char Packet_CNT = 0;
unsigned char Byte_Number = 0;
unsigned char PacketData[20][42] = { 0 };
unsigned char NextCMD_Number = 0;
unsigned char Length[20] = { 0 };
unsigned char CMD_Number = 0;

unsigned char CMD_Pos = 0;

unsigned char Sync_flag = 0;
unsigned int RTC_ratio = 100;
unsigned int FirstTemp_ratio = 100;
unsigned int Temp_ratio = 0;
unsigned int RTC_div = 100;
unsigned int Start_Sync_CNT_value = 0;

unsigned char CC_ready_flag = 0;

// --------IR-----------------------//
#ifdef IR
extern unsigned char IR_event;
extern unsigned char ChangeStateEvent;
extern irHeader_t NewIrHeader;
#endif
//---------PIR----------------------//
#ifdef PIR
unsigned char PirNwkStatus = NwkEmpty;
extern unsigned char RlStatusFlag;
extern unsigned char Pir_status;

#endif

void CompleteCommand(void) {
	SCI0_C2_RE = 1;	// khi xu li xong 1 ban tin, bat cho phep nhan ban tin tiep
	CMD_Number--;							// so ban tin con lai chua dc xu ly
	if (CMD_Pos >= CMD_Number) {
		NextCMD_Number = CMD_Pos - CMD_Number; // neu so ban tin chua duoc xu li nho hon so thu tu ban tin
	} else {
		NextCMD_Number = 20 + CMD_Pos - CMD_Number;	// co ban tin chua dc xu ly lon so thu tu ban tin--> quay tro lai cuoi
	}
}
void CC_BD_MCSendVertion(void) {
	unsigned char check;
	check = 0xAD ^ CodeType ^ CodeOpption ^ (CodeVer >> 8) ^ CodeVer;
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(6);
	UART0_putchar(0xAD);
	UART0_putchar(CodeType);
	UART0_putchar(CodeOpption);
	UART0_putchar(CodeVer >> 8);
	UART0_putchar(CodeVer);
	UART0_putchar(check);
}
void CC_send_ReadyMC(void) {
	unsigned char i = 0;
	unsigned char Check = 0;
	unsigned char CheckData[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
			0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
	Check = 0xD0;
	for (i = 0; i < 16; i++) {
		Check = Check ^ CheckData[i];
	}
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(0x12);
	UART0_putchar(0xD0);
	for (i = 0; i < 16; i++) {
		UART0_putchar(CheckData[i]);
	}
	UART0_putchar(Check);
}

void CC_send_SynClock(void) {
	unsigned char i = 0;
	unsigned char Check = 0;
	unsigned char CheckData[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
			0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
	Check = 0xD1;
	for (i = 0; i < 16; i++) {
		Check = Check ^ CheckData[i];
	}
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(18);
	UART0_putchar(0xD1);
	for (i = 0; i < 16; i++) {
		UART0_putchar(CheckData[i]);
	}
	UART0_putchar(Check);
}

void CC_C1_SyncClock(unsigned char *s){
	unsigned char i = 0;
	unsigned char CheckData[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
			0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
	for(i=0;i<16;i++){
		if(s[1+i] != CheckData[i]){
			CC_ready_flag = 0;
			return;
		}
	}
	CC_ready_flag = 1;
	CC_send_ReadyMC();
}
void CC_B4_ReadEEPROM(unsigned char *s) {
	unsigned char Start_addr;
	unsigned char Read_length;
	unsigned int i;
	unsigned char Check = 0;

	unsigned char Write_length = 0;
	unsigned char temp[40] = { 0 };

	Start_addr = s[1];
	Read_length = s[2];
	Write_length = Read_length + 4;
	Check = 0xA4 ^ Start_addr ^ Read_length;
	for (i = 0; i < Read_length; i++) {
		temp[i] = EEPROM_Read(0x3100 + Start_addr + i);			//data
		Check = Check ^ temp[i];
	}
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);				// key
	UART0_putchar(Write_length);		//length
	UART0_putchar(0xA4);				//ID
	UART0_putchar(Start_addr);			//start addr
	UART0_putchar(Read_length);			//read length	
	UART0_SendBurst(temp, Read_length);
	UART0_putchar(Check);				//check xor
}
void CC_B5_WriteEEPROM(unsigned char *s) {
	unsigned char Start_addr;
	unsigned char Write_length;
	unsigned int i;
	unsigned char Check = 0;
	unsigned char EEPROM_write_data[40];
	unsigned char EEPROM_check[40];
	unsigned char cnt = 0;

	Start_addr = s[1];
	Write_length = s[2];
	for (i = 0; i < 40; i++) {
		EEPROM_write_data[i] = s[3 + i];
	}
	Check = 0xA5 ^ Start_addr ^ Write_length;

	EEPROM_EraseByteBurst((0x3100 + Start_addr), Write_length);

	for (i = 0; i < Write_length; i++) {
		EEPROM_Write((0x3100 + Start_addr + i), EEPROM_write_data[i]);
	}
	for (i = 0; i < Write_length; i++) {
		EEPROM_check[i] = EEPROM_Read(0x3100 + Start_addr + i);			//data
	}
	for (i = 0; i < Write_length; i++) {
		if (EEPROM_check[i] == EEPROM_write_data[i]) {
			cnt++;
		}
	}
	if (cnt == Write_length) {
		UART0_putchar(0x4C);
		UART0_putchar(0x4D);
		UART0_putchar(4);
		UART0_putchar(0xA5);
		UART0_putchar(Start_addr);
		UART0_putchar(Write_length);
		UART0_putchar(Check);
	}
}
void CC_B6_EraseEEPROM(unsigned char *s) {
	unsigned char Start_addr;
	unsigned int Erase_length;
	//	unsigned int i;
	unsigned char Check = 0;

	Start_addr = s[1];
	Erase_length = (s[2] << 8) + s[3];
	Check = 0xA6 ^ Start_addr ^ s[2] ^ s[3];

	EEPROM_EraseByteBurst((0x3100 + Start_addr), Erase_length);

	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(5);
	UART0_putchar(0xA6);
	UART0_putchar(Start_addr);
	UART0_putchar((unsigned char) (Erase_length >> 8));
	UART0_putchar((unsigned char) (Erase_length & 0xFF));
	UART0_putchar(Check);

}
void CC_B7_LedControlIrPir(unsigned char *s){
	unsigned char State;
	unsigned char Extra;
	unsigned char i;
	unsigned char blink_command_flag = 0;
	unsigned char Led_mode;
	unsigned char RL_mode;
	unsigned char BlinkTime;
	State = s[1];
	Extra = s[2];
	switch ((State ) & 0x03) {
		case 0b00:
			break;
		case 0b01:
			StatusLedOff
			break;
		case 0b10:
			StatusLedOn
			break;
		case 0b11:
			Led_mode = (Extra >> 3) & 0x03;
			RL_mode = (Extra >> 1) & 0x03;
			BlinkTime = (2 * (Extra >> 5)) + 1;
			for(i = 0; i <BlinkTime; i++){
				StatusLedBlink500ms;
			}
			break;
		default:
			break;
	}
	switch(Led_mode){
		case 0b00:
			break;
		case 0b01:
			StatusLedOff
			break;
		case 0b10:
			StatusLedOn
			break;
		case 0b11:
			break;
	}
}
void CC_BE_SyncTime(unsigned char *s) {
	unsigned char i = 0;
	unsigned char total_time = 0;
	unsigned char cnt_time = 0;
	unsigned char interval = 0;
	unsigned int temp_abs = 0;

	total_time = s[2];
	interval = s[1];
	cnt_time = s[3];
	if ((Sync_flag == cnt_time)) {
		if (Sync_flag == 0) {
			Start_Sync_CNT_value = RTC_CNT;
			Sync_flag++;
		} else if (Sync_flag == 1) {
			if (RTC_CNT >= Start_Sync_CNT_value) {
				FirstTemp_ratio = RTC_CNT - Start_Sync_CNT_value;
			} else {
				FirstTemp_ratio = 0xFFFF - Start_Sync_CNT_value + RTC_CNT;
			}
			Start_Sync_CNT_value = RTC_CNT;
			Sync_flag++;
		} else if (Sync_flag > 1) {
			if (RTC_CNT >= Start_Sync_CNT_value) {
				Temp_ratio = RTC_CNT - Start_Sync_CNT_value;
			} else {
				Temp_ratio = 0xFFFF - Start_Sync_CNT_value + RTC_CNT;
			}
			temp_abs = abs2int(Temp_ratio, FirstTemp_ratio);
			if (temp_abs < 3) {
				Sync_flag++;
			}
			Start_Sync_CNT_value = RTC_CNT;
		}

		if (cnt_time == total_time) {
			if (total_time > 1) {
				if (temp_abs < 3) {
					RTC_div = interval;
					RTC_ratio = FirstTemp_ratio;
				}
			} else if (total_time == 1) {
				RTC_div = interval;
				RTC_ratio = FirstTemp_ratio;
			}
			Sync_flag = 0;
		}
	} else {
		Sync_flag = 0;
		if (cnt_time == 0) {
			Start_Sync_CNT_value = RTC_CNT;
			Sync_flag++;
		}
	}
}
unsigned char xor_int(unsigned int intnumber) {
	unsigned char temp = 0;
	temp = (intnumber >> 8) ^ intnumber;
	return temp;
}
unsigned char xor_str(unsigned char *s, unsigned int length) {
	unsigned int i;
	unsigned char temp = 0;
	for (i = 0; i < length; i++) {
		temp = temp ^ s[i];
	}
	return temp;
}










//--------------------------------------------------------------------------------------//
//-------------------------------------IR COMANDER--------------------------------------//
//--------------------------------------------------------------------------------------//

#ifdef IR



void CC_D3_SendIrStatus(unsigned char IR_State, irHeader_t tempIrHeader) {
	unsigned char check;
	check = 0xD3 ^ IR_State ^ xor_int(tempIrHeader.remoteID)
			^ xor_int(tempIrHeader.cmdRemotePos) ^ xor_int(tempIrHeader.no);
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(9);
	UART0_putchar(0xD3);
	UART0_putchar(IR_State);
	UART0_putstr(&tempIrHeader.remoteID, sizeof(tempIrHeader.remoteID));
	UART0_putstr(&tempIrHeader.cmdRemotePos, sizeof(tempIrHeader.cmdRemotePos));
	UART0_putstr(&tempIrHeader.no, 2);

	UART0_putchar(check);
}
void CC_D5_SendIrCmdInfo(unsigned int IR_no) {
	unsigned char check;
	irHeader_t tempIrHeader;
	flash_read(IR_no * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
	check = 0xD5 ^ xor_str(&tempIrHeader, sizeof(tempIrHeader));
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(sizeof(tempIrHeader) + 2);
	UART0_putchar(0xD5);
	UART0_putstr(&tempIrHeader, sizeof(tempIrHeader));
	UART0_putchar(check);
}
void CC_D7_SendIrTotalCmd(void) {
	unsigned char check;
	irHeader_t tempIrHeader;
	unsigned int i;
	unsigned int j;
	unsigned int TotalIr = 0;
	unsigned int UsedIr = 0;
	unsigned int TotalIrType[MAX_REMOTE_TYPE] = { 0 };
	unsigned int NumberType = 0;
	unsigned int cmdTypeCnt = 0;
	unsigned char length;
	cmdType_t cmdType[MAX_REMOTE_TYPE] = { 0 };

	TotalIr = MAX_FLASH_SECTOR;
	
	for (i = 0; i < MAX_FLASH_SECTOR; i++) {
		flash_read(i * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
		if (i == tempIrHeader.no) {
			for (j = 0; j < MAX_REMOTE_TYPE; j++) {
				if (j == tempIrHeader.remoteType) {
					TotalIrType[j]++;
				}
			}
			UsedIr++;
		}
	}

	for (i = 0; i < MAX_REMOTE_TYPE; i++) {
		if (TotalIrType[i] != 0) {
			cmdType[cmdTypeCnt].type = i;
			cmdType[cmdTypeCnt].total = TotalIrType[i];
			cmdTypeCnt++;
			NumberType = cmdTypeCnt;
		}
	}
	check = 0xD7 ^ xor_int(TotalIr) ^ xor_int(UsedIr);
	for (i = 0; i < cmdTypeCnt; i++) {
		check = check ^ xor_str(&cmdType[i], sizeof(cmdType[i]));
	}

	length = (unsigned char) (6 + 4 * NumberType);

	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(length);
	UART0_putchar(0xD7);
	UART0_putstr(&TotalIr, 2);
	UART0_putstr(&UsedIr, 2);
	for (i = 0; i < NumberType; i++) {
		UART0_putstr(&cmdType[i], sizeof(cmdType[i]));
	}
	UART0_putchar(check);
}
void CC_DA_SendMaxIdRemote(void) {
	unsigned int i;
	unsigned int MaxRemoteID = 0;
	unsigned char check;
	unsigned char IrCmdStored_Flag = 0;

	irHeader_t tempIrHeader;
	for (i = 0; i < MAX_FLASH_SECTOR; i++) {
		flash_read(i * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
		if (i == tempIrHeader.no) {
			if (tempIrHeader.remoteID >= MaxRemoteID) {
				MaxRemoteID = tempIrHeader.remoteID;
			}
			IrCmdStored_Flag = 1;
		}
	}
	if (IrCmdStored_Flag == 0) {
		MaxRemoteID = 0;
	} else {
		MaxRemoteID += 1;
	}
	check = 0xDA ^ xor_int(MaxRemoteID);
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(4);
	UART0_putchar(0xDA);
	UART0_putstr(&MaxRemoteID, 2);
	UART0_putchar(check);
}

void CC_C3_LearnIRCmd(unsigned char *s) {
	unsigned int i;
	irHeader_t TempIrHeader;

	//quet bo nho,
	//tim vi tri dat lenh
	for (i = 0; i < MAX_FLASH_SECTOR; i++) {
		flash_read(ByteInPage * i, &TempIrHeader, sizeof(TempIrHeader));
		if ((i != TempIrHeader.no)) {
			break;
		}
	}
	//luu header lenh vao bien luu lenh moi tam thoi. 

	NewIrHeader.no = i;
	NewIrHeader.remoteID = s[1] << 8 | s[2];
	NewIrHeader.remoteType = s[3] << 8 | s[4];
	NewIrHeader.cmdRemotePos = s[5] << 8 | s[6];

	// neu bo nho full --> gui lenh, va chuyen sang che do cho lenh
	if (i == (MAX_FLASH_SECTOR - 1)) {
		CC_D3_SendIrStatus(MemoryFull, NewIrHeader);
		IR_event = WaitCCCmd;
		return;
	}
	flash_sector_erase(NewIrHeader.no * ByteInPage);
	//chuyen trang thai main sang hoc lenh

	IR_event = LearnIRCmd;
	ChangeStateEvent = 1;
	//bat dau 60s hoc lenh tai main

}
void CC_C4_EraserIRCmd(unsigned char *s) {
	unsigned int IR_no;
	irHeader_t tempIrHeader;
	IR_no = s[1] << 8 | s[2];
	flash_read(IR_no * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
	tempIrHeader.no = IR_no;
	CC_D3_SendIrStatus(ErraseCmdDone, tempIrHeader);
	flash_sector_erase(IR_no * ByteInPage);
}
void CC_C5_ReadIRCmd(unsigned char *s) {
	unsigned int IR_no;
	IR_no = s[1] << 8 | s[2];
	CC_D5_SendIrCmdInfo(IR_no);
}
void CC_C6_SetIRCmd(unsigned char *s) {
	unsigned int IR_no;
	irHeader_t tempIrHeader;
	IR_no = s[1] << 8 | s[2];
	flash_read(IR_no * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));

	if ((tempIrHeader.no == IR_no) && (CheckIrDataStore(tempIrHeader))) {
		CC_D3_SendIrStatus(RunCmdDone, tempIrHeader);
		IrSendCmd(IR_no * ByteInPage + sizeof(tempIrHeader),
				tempIrHeader.cmdLen);
	} else {
		CC_D3_SendIrStatus(CmdNotHave, tempIrHeader);
	}
}
void CC_C7_ReadIRCmdNumber(void) {
	CC_D7_SendIrTotalCmd();
}
void CC_C8_StopLearnIRCmd(void) {
	IR_event = LearnTimeout;
}

void CC_C9_AddIrCmdInfo(unsigned char *s) {
	unsigned int IR_no;
	unsigned int i;
	unsigned int tempData;
	irHeader_t tempIrHeader;
	IR_no = s[1] << 8 | s[2];
	
	// doc cmd info cu
	flash_read(IR_no * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
	//lay thong tin header moi
	for (i = 0; i < 32; i++) {
		tempIrHeader.cmdName[i] = s[3 + i];
	}
	for (i = 0; i < 4; i++) {
		tempIrHeader.cmdInfo[i] = s[35 + i];
	}
	
	// xoa bo nho tam
	flash_sector_erase(TempFlashStore * ByteInPage);
	// luu header vao bo nho tam
	flash_page_program(TempFlashStore * ByteInPage, &tempIrHeader,
			sizeof(tempIrHeader));
	// luu lenh vao bo nho tam
	for (i = 0; i < tempIrHeader.cmdLen; i++) {
		flash_read(tempIrHeader.no * ByteInPage + sizeof(tempIrHeader) + 2 * i,
				&tempData, sizeof(tempData));
		flash_page_program(
				TempFlashStore * ByteInPage + sizeof(tempIrHeader) + 2 * i,
				&tempData, sizeof(tempData));
	}
	// xoa toan bo header va lenh cu
	flash_sector_erase(IR_no * ByteInPage);
	// luu toan bo header va lenh moi vao dia chi lenh cu
	flash_page_program(IR_no * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
	for (i = 0; i < tempIrHeader.cmdLen; i++) {
		flash_read(TempFlashStore * ByteInPage + sizeof(tempIrHeader) + 2 * i,
				&tempData, sizeof(tempData));
		flash_page_program(
				tempIrHeader.no * ByteInPage + sizeof(tempIrHeader) + 2 * i,
				&tempData, sizeof(tempData));
	}
	CC_D3_SendIrStatus(GetExtraInforAndStoreDone, tempIrHeader);
}

void CC_CA_FindMaxIdRemote(void) {

	CC_DA_SendMaxIdRemote();
}

void CC_CB_EraseIdRemote(unsigned char *s) {
	unsigned int IdRemoteNo = 0;
	unsigned int i;
	irHeader_t tempIrHeader;
	IdRemoteNo = s[1] << 8 | s[2];

	for (i = 0; i < MAX_FLASH_SECTOR; i++) {
		flash_read(i * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
		if (tempIrHeader.remoteID == IdRemoteNo) {
			flash_sector_erase(i * ByteInPage);
		}
	}
	tempIrHeader.remoteID = IdRemoteNo;
	CC_D3_SendIrStatus(ErraseCmdDone, tempIrHeader);
}

void CC_SendAllIrFlashStore(void){
	unsigned int i;
	irHeader_t tempIrHeader;
	unsigned int j;
	unsigned char *tempstr;
	unsigned int tempFlashData;
	for (i = 0; i < MAX_FLASH_SECTOR; i++) {
		flash_read(i * ByteInPage, &tempIrHeader, sizeof(tempIrHeader));
		if (i == tempIrHeader.no) {
			(void)sprintf(tempstr,"Header no %u, \n\r",tempIrHeader.no);
			UART0_puts(tempstr);
			(void)sprintf(tempstr,"remote ID %u, \n\r",tempIrHeader.remoteID);
			UART0_puts(tempstr);
			(void)sprintf(tempstr,"remote type %u, \n\r",tempIrHeader.remoteType);
			UART0_puts(tempstr);
			(void)sprintf(tempstr,"Cmd Remote Position %u, \n\r",tempIrHeader.cmdRemotePos);
			UART0_puts(tempstr);
			(void)sprintf(tempstr,"Cmd Length %u, \n\r",tempIrHeader.cmdLen);
			UART0_puts(tempstr);
			(void)sprintf(tempstr,"Cmd Name %s, \n\r",tempIrHeader.cmdName);
			UART0_puts(tempstr);
			(void)sprintf(tempstr,"remote type %s, \n\r",tempIrHeader.cmdInfo);
			UART0_puts(tempstr);
			UART0_puts("Cmd Data");
			for(j=0;j<tempIrHeader.cmdLen; j++){
				flash_read(tempIrHeader.no * ByteInPage + sizeof(tempIrHeader) + 2 * i,
						&tempFlashData, sizeof(tempFlashData));
				UART0_puti(tempFlashData);
				UART0_puts("  ");
			}
		}
	}
}
#endif

//--------------------------------------------------------------------------------------//
//-------------------------------------PIR COMANDER-------------------------------------//
//--------------------------------------------------------------------------------------//

#ifdef PIR


void CC_F0_PirSendData(void) {

#ifdef CC_Interface
	unsigned char check;
	PirStatus_t PirStatus;
	PirStatus.PIRStatus = Pir_status;
	PirStatus.LightSensor = I2C_ReadLightValue()*3/2;
	PirStatus.LightLevel = CalcLightVar()*3/2;
	PirStatus.TimeDelay = CalcTimeOut();

	check = 0xF0 ^ xor_str(&PirStatus, sizeof(PirStatus));
	UART0_putchar(0x4C);
	UART0_putchar(0x4D);
	UART0_putchar(sizeof(PirStatus) + 2);
	UART0_putchar(0xF0);
	UART0_putstr(&PirStatus, sizeof(PirStatus));
	UART0_putchar(check);

#endif
}
void CC_E0_CCGetPirStatus(void) {

	CC_F0_PirSendData();

}
void CC_E1_CCSendDataDone(void) {
	//Turn_off CC
}

#endif

void CC_receiver_program(void) {
	unsigned char Command_ID;
	unsigned char Check_XOR;
	unsigned char Check = 0x00;
	unsigned char j = 0;
	unsigned int temp_value = 0;

//	Time_SyncSend();
	if (CMD_Number == 0) {
	} else {
		Check_XOR = PacketData[NextCMD_Number][Length[NextCMD_Number] - 1];
		for (j = 0; j < Length[NextCMD_Number] - 1; j++) {
			Check = Check ^ PacketData[NextCMD_Number][j];
		}
		if (Check == Check_XOR) {

			Command_ID = PacketData[NextCMD_Number][0];
			switch (Command_ID) {
			//-----------------------Dieu khien cong tac 1-4----------------------------//
//			case 0xB0:
//
//				break;
//				//-----------------------Dieu khien Rem------------------------------------//
//			case 0xB1:
//
//				break;
//				//-----------------------Dieu khien Dimmer---------------------------------//
//			case 0xB2:
//							
//				break;
//				//------------------------Dieu khien quat----------------------------------//		
//			case 0xB3:
//		
//				break;
				//------------------------Doc du lieu EEPROM-------------------------------//
			case 0xB4:
				CC_B4_ReadEEPROM(PacketData[NextCMD_Number]);
				break;
				//------------------------Ghi du lieu EEPROM------------------------------//
			case 0xB5:
				CC_B5_WriteEEPROM(PacketData[NextCMD_Number]);
				break;
				//------------------------Xoa du lieu EEPROM-------------------------------//
			case 0xB6:
				CC_B6_EraseEEPROM(PacketData[NextCMD_Number]);
				break;
				//------------------------ Dieu khien led sang trang thai dac biet--------//
			case 0xB7:
				CC_B7_LedControlIrPir(PacketData[NextCMD_Number]);
				break;
//				//------------------------Dieu khien dac biet/ Cau hinh rem-------------------------------//		
//			case 0xB8:
//
//				break;
//				//------------------------Cau hinh toc do DIM-------------------------------//			
//			case 0xB9:
//
//				break;
//				//-------------------------Tuy chon chuc nang-------------------------------//
//			case 0xBC:
//
//				break;
//				//-------------------------Gui ban tin loi-----------------------------------//
//			case 0xBA:
//
//				break;
			//-------------------------Kiem tra version MC-----------------------------------//
			case 0xBD:
				CC_BD_MCSendVertion();
				break;
				//-------------------------Dong bo RTC timer-----------------------------------//
			case 0xBE:
				CC_BE_SyncTime(PacketData[NextCMD_Number]);
				break;
				//-------------------------Bao hieu MC san sang-----------------------------------//
			case 0xC0:
				CC_send_ReadyMC();
				break;
			case 0xC1:
				CC_C1_SyncClock(PacketData[NextCMD_Number]);
				break;

#ifdef IR				
				//-------------------------Hoc lenh IR moi-----------------------------------//
			case 0xC3:
				StatusLedBlink
				;
				CC_C3_LearnIRCmd(PacketData[NextCMD_Number]);
				break;
				//-------------------------Xoa lenh IR da hoc-----------------------------------//
			case 0xC4:
				StatusLedBlink
				;
				CC_C4_EraserIRCmd(PacketData[NextCMD_Number]);
				break;
				//-------------------------Doc lenh IR da hoc-----------------------------------//
			case 0xC5:
				CC_C5_ReadIRCmd(PacketData[NextCMD_Number]);
				break;
				//-------------------------Phat lenh IR da hoc----------------------------------//
			case 0xC6:
				StatusLedBlink
				;
				CC_C6_SetIRCmd(PacketData[NextCMD_Number]);
				break;
				//-------------------------Doc so lenh da luu----------------------------------//
			case 0xC7:
				CC_C7_ReadIRCmdNumber();
				break;
				//-------------------------Dung qua trinh hoc lenh----------------------------------//
			case 0xC8:
				CC_C8_StopLearnIRCmd();
				break;
				//-------------------------Bo sung thong tin lenh IR----------------------------------//
			case 0xC9:
				StatusLedBlinkLongtime;
				CC_C9_AddIrCmdInfo(PacketData[NextCMD_Number]);
				
				break;
				//-------------------------Tim chi so ID remote lon nhat----------------------------------//
			case 0xCA:
				CC_CA_FindMaxIdRemote();
				break;
				//-------------------------Xoa tat ca lenh thuoc mot ID remote----------------------------------//
			case 0xCB:
				StatusLedBlink
				;
				CC_CB_EraseIdRemote(PacketData[NextCMD_Number]);
				break;
			case 0xDD:
				CC_SendAllIrFlashStore();
				break;
#endif
#ifdef PIR
				//-------------------------Yeu cau tra loi trang thai PIR----------------------------------//
				case 0xE0:
				CC_E0_CCGetPirStatus();
				break;

				//-------------------------CC bao gui du lieu xong----------------------------------//
				case 0xE1:
				CC_E1_CCSendDataDone();
				break;
#endif				
			default:
				break;
				//------------------------END-------------------------------//
			}
		}
		CompleteCommand();
	}
}
interrupt VectorNumber_Vsci0rx void sci0rx(void) {

	unsigned char dummy = 0;
	//	unsigned char data=0;
	unsigned char ReceiverData = 0;
	dummy = SCI0_S1;
	ReceiverData = SCI0_D;

	//	UART0_putchar(ReceiverData);
	//	__delay_us(100);

	switch (Byte_Number) {
	case 0:
		if (ReceiverData == 0x4C) {
			Byte_Number = 1;
		} else {
			Byte_Number = 0;
		}
		break;
	case 1:
		if (ReceiverData == 0x4D) {
			Byte_Number = 2;
		} else {
			Byte_Number = 0;
		}
		break;
	case 2:
		Length[CMD_Pos] = ReceiverData;
		if (Length[CMD_Pos] > 50) {
			Length[CMD_Pos] = 0;
			Byte_Number = 0;
		}
		Byte_Number = 3;
		break;
	case 3:
		PacketData[CMD_Pos][Packet_CNT] = ReceiverData;
		Packet_CNT++;
		if (Packet_CNT >= Length[CMD_Pos]) {
			Packet_CNT = 0;
			Byte_Number = 0;
			CMD_Pos++;								// vi tri luu ban tin toi
			if (CMD_Pos >= 20) {
				CMD_Pos = 0;
			}
			CMD_Number++;
			if (CMD_Number >= 20) {					// so ban tin chua dc xu ly
				SCI0_C2_RE = 0;				// neu qua 20, khong cho nhan nua
			}
		}
		break;
	default:
		break;
	}
}

