/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file SPI.c
*
* @author B37531
*
* @version 0.0.1
*
* @date Aug 23, 2011
*
* @brief application entry point which performs application specific tasks. 
*
******************************************************************************/

#ifndef SPI_H_
#define SPI_H_

/******************************************************************************
* Macros
******************************************************************************/
#define CS_PIN_Init		PORT_PTBOE_PTBOE5 = 1
#define DESELECT()			PORT_PTBD_PTBD5 = 1
#define SELECT()			PORT_PTBD_PTBD5 = 0
/******************************************************************************
* Global functions
******************************************************************************/
void SPI0_Init( void );
void SPI1_Init( void );
void SPI0_RdWrByte( unsigned char* pWrBuff,unsigned char* pRdBuff,unsigned short uiLength );

 


/*********************************************************************
 * CONSTANTS
 */
#define	WRITE_ENABLE					0x06
#define	WRITE_DISABLE					0x04
#define	READ_STAT							0x05
#define	WRITE_STAT						0x01
#define	READ_DATA							0x03
#define	FAST_READ							0x0B
#define	FAST_READ_DUAL				0x3B
#define	PAGE_PROGRAM					0x02
#define	BLOCK_ERASE						0xD8
#define	SECTOR_ERASE					0x20
#define	CHIP_ERASE						0xC7
#define	POWER_DOWN						0xB9
#define	DEVICE_ID							0x90
#define	READ_ID								0x9F



#define	FLASH_NUM_OF_SECTOR			128
#define	FLASH_SECTOR_SIZE				4096
#define	FLASH_SECTOR_SHIFT_BV		12
#define	FLASH_PAGE_SIZE					256

/***************************************************************************************************
 *                                              MACROS
 ***************************************************************************************************/


/***************************************************************************************************
 *                                              FUNCTIONS
 ***************************************************************************************************/
void flash_init(void);
void flash_read(unsigned long address, unsigned char* p, unsigned int cnt);
void flash_block_erase(unsigned long address);
void flash_sector_erase(unsigned long address);
void flash_page_program(unsigned long address, unsigned char* p,unsigned int cnt);
void flash_erase(void);
unsigned char flash_read_stat(void);
void flash_WIP_poll(void);
void flash_read_id(unsigned char* p);
void TestFlash(void);


/***************************************************************************************************
***************************************************************************************************/



#endif /* SPI_H_ */
