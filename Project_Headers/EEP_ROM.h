/*
 * eeprom.h
 *
 *  Created on: Aug 7, 2014
 *      Author: DangGiap
 */



#ifndef EEP_ROM_H_
#define EEP_ROM_H_
#include "Device.h"


#define RAM_CODE_START		0x0F3F

/* Flash driver errors */
#define FLASH_ERR_BASE				0x3000
#define FLASH_ERR_SUCCESS			0
#define FLASH_ERR_INVALID_PARAM		(FLASH_ERR_BASE+1)	// invalid parameter error code
#define EEPROM_ERR_SINGLE_BIT_FAULT	(FLASH_ERR_BASE+2)	// EEPROM single bit fault error code
#define EEPROM_ERR_DOUBLE_BIT_FAULT	(FLASH_ERR_BASE+4)	// EEPROM double bits fault error code
#define FLASH_ERR_ACCESS			(FLASH_ERR_BASE+8)	// flash access error code
#define FLASH_ERR_PROTECTION		(FLASH_ERR_BASE+0x10)	// flash protection error code
#define FLASH_ERR_MGSTAT0			(FLASH_ERR_BASE+0x11)	// flash verification error code
#define FLASH_ERR_MGSTAT1			(FLASH_ERR_BASE+0x12)	// flash non-correctable error code
#define FLASH_ERR_INIT_CCIF			(FLASH_ERR_BASE+0x14)	// flash driver init error with CCIF = 1
#define FLASH_ERR_INIT_FDIV			(FLASH_ERR_BASE+0x18)	// flash driver init error with wrong FDIV 

/* Flash and EEPROM commands */
#define ERASE_VERIFY_ALL	0x01
#define ERASE_VERIFY_BLOCK	0x02
#define ERASE_ALL			0x08
#define ERASE_BLOCK			0x09
#define UNSECURE			0x0B
#define SET_USER_MARGIN		0x0D

#define FLASH_ERASE_VERIFY_SECTION	0x03
#define FLASH_READONCE			0x04
#define FLASH_PROGRAM			0x06
#define FLASH_PROGRAMONCE		0x07
#define FLASH_ERASE_SECTOR		0x0A
#define FLASH_BACKDOOR_ACCESS	0x0B

#define EEPROM_ERASE_VERIFY_SECTION	0x10
#define EEPROM_PROGRAM			0x11
#define EEPROM_ERASE_SECTOR 	0x12

/*************************************************************
 * Doc 1 byte EEPROM
 * Tham so la dia chi can doc
 * Tra ve gia tri doc duoc
 *************************************************************/
unsigned char EEPROM_Read(int address_EEPROM);

/*************************************************************
 * Doc nhieu Byte EEPROM
 * Dia chi Byte dau va so byte can doc
 *************************************************************/
void EEPROM_ReadBurst(int address_EEPROM, unsigned char *buffer, unsigned char size);

/*************************************************************
 * Ghi 1 byte EEPROM
 * Tham so la dia chi thanh ghi va gi tri can ghi
**************************************************************/
void EEPROM_Write(int address_EEPROM, unsigned char data_EEPROM);

/************************************************************
 * Ghi nhieu Byte
 * 
 ***********************************************************/
void EEPROM_WriteBurst(int address_EEPROM, unsigned char buffer[256], unsigned size);

/**********************************************************
 * Xoa 1 byte EEPROM
 * Tham so truyen vao la dia chi thanh ghi
 * 
 ***********************************************************/
void EEPROM_EraseByte(int address_EEPROM);

/**********************************************************
 * Xoa toan bo EEPROM 
 **********************************************************/
void EEPROM_EraseAll(int address_EEPROM);

/**********************************************************
 * Kiem tra EEPROM da format chua 
 **********************************************************/
unsigned char EEPROM_FormatFlag(void);

/**********************************************************
 * Doc 1 byte FLASH
 * Tham so truyen vao la dia chi thanh ghi
 * Tra ve gi tri doc duoc
 **********************************************************/
unsigned char FLASH_Read(int address_FLASH);

/**********************************************************
 * Doc nhieu BYTE FLASH
 * Dia chi Byte dau va so luong byte
 **********************************************************/
void FLASH_ReadBurst( int address_FLASH, unsigned char *buffer, unsigned char size);

/**********************************************************
 * Ghi 1 byte FLASH
 * Tham so truyen vao la dia chi thanh ghi va gi tri can ghi
 **********************************************************/
void FLASH_Write(int address_FLASH, unsigned char data_FLASH);

/**********************************************************
 * Xoa 1 byte FLASH
 **********************************************************/
void FLASH_EraseByte(int address_FLASH);

/**********************************************************
 * Xoa ca 2 vung nho FLASH va EEPROM
 * 
 **********************************************************/
void EraseAll(void);

unsigned int EEPROM_Read_int( int address_EEPROM);
void EEPROM_Write_int ( int address_EEPROM, unsigned int data);
void EEPROM_EraseBlock(int address_EEPROM);
void EEPROM_EraseVerifyByteBurst(int address_EEPROM, unsigned char size);
void EEPROM_EraseByteBurst(int address_EEPROM, unsigned int leng);
#endif /* EEPROM_H_ */
