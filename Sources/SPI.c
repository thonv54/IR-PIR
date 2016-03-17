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
#include "SPI.h"
#include "Device.h"

/*****************************************************************************//*!
 +FUNCTION----------------------------------------------------------------
 * @function name: SPI0_Init
 *
 * @brief initialize SPI0 to master mode
 *        
 * @param  none 
 *
 * @return none
 *
 * @ Pass/ Fail criteria: none
 *****************************************************************************/
void SPI0_Init(void) {
	unsigned char temp[4] = { 0 };
	volatile char dummy;

	CS_PIN_Init;
	// SPI0 initial
	//SYS_SOPT1_SPI0PS  = 1;     // SPI0 SPSCK0, MOSI0, MISO0, and SS0 are mapped on PTE0, PTE1, PTE2, and PTE3.
	SPI0_C1_MSTR = 1;            // master
	SPI0_C1_CPOL = 0;
	SPI0_C1_CPHA = 0;

	SPI0_BR_SPR = 0b001;		// device 2
	SPI0_BR_SPPR = 0b000;		// device 2 -> Baud = 20/4 =500khz
	dummy = SPI0_D;
	SPI0_C1_SPE = 1;            // enabled

	flash_read_id(temp);
}

/*****************************************************************************//*!
 +FUNCTION----------------------------------------------------------------
 * @function name: SPI0_RdWrByte
 *
 * @brief implement read/write data from SPI0
 *        
 * @param  *pWrBuff   -- write data buffer pointer 
 *         *pRdBuff   -- read  data buffer pointer
 *         uiLength   -- read/write data length
 *
 * @return none
 *
 * @ Pass/ Fail criteria: none
 *****************************************************************************/
void SPI0_RdWrByte(unsigned char* pWrBuff, unsigned char* pRdBuff,
		unsigned short uiLength) {
	unsigned short i;
	for (i = 0; i < uiLength; i++) {
		SPI0_S |= SPI0_S_SPTEF_MASK;
		SPI0_D = pWrBuff[i];
		while (!SPI0_S_SPRF)
			;
		pRdBuff[i] = SPI0_D;
	}
}

/*********************************************************************
 * @fn          spi_rw
 *
 * @brief      	spi basic function to transmit and receive data
 *
 * @param       data: data to transmit
 *
 * @return      received data
 */
unsigned char spi_rw(unsigned char data) {
	SPI0_S |= SPI0_S_SPTEF_MASK;
	SPI0_D = data;
	while (!SPI0_S_SPRF)
		;
	return SPI0_D;
}

/*********************************************************************
 * @fn          flash_cmd
 *
 * @brief      	write cmd header and its header param
 *
 * @param       cmd:		CMD ID
 *							param:	param for cmd
 *
 * @return      received data
 */
void flash_cmd(unsigned char cmd, unsigned long param) {
	unsigned char temp;
	DESELECT();
	SELECT();
	temp = spi_rw(cmd);
	switch (cmd) {
	case READ_DATA:
	case PAGE_PROGRAM:
		temp = spi_rw((unsigned char) (param >> 16));
		temp = spi_rw((unsigned char) (param >> 8));
		temp = spi_rw((unsigned char) (param));
		break;

	case BLOCK_ERASE:
	case SECTOR_ERASE:
		temp = spi_rw((unsigned char) (param >> 16));
		temp = spi_rw((unsigned char) (param >> 8));
		temp = spi_rw((unsigned char) (param));
		DESELECT();
		break;

	case WRITE_ENABLE:
	case WRITE_DISABLE:
	case CHIP_ERASE:
	case POWER_DOWN:
		DESELECT();
		break;

	case READ_ID:
		break;
	}
}

/*********************************************************************
 * @fn          flash_write_enable
 *
 * @brief      	enable write to flash
 *
 * @param       none
 *
 * @return      none
 */
void flash_write_enable() {
	flash_cmd(WRITE_ENABLE, 0);
}

/*********************************************************************
 * @fn          flash_write_disable
 *
 * @brief      	disable write to flash
 *
 * @param       none
 *
 * @return      none
 */
void flash_write_disable() {
	flash_cmd(WRITE_DISABLE, 0);
}

/*********************************************************************
 * @fn          flash_read_stat
 *
 * @brief      	read flash status register
 *
 * @param       none
 *
 * @return      stat value
 */
unsigned char flash_read_stat(void) {
	unsigned char rcv;
	flash_cmd(READ_STAT, 0);

	rcv = spi_rw(0xFF);

	DESELECT();
	return rcv;
}

/*********************************************************************
 * @fn          flash_WIP_poll
 *
 * @brief      	check and wait until write process is done
 *
 * @param       none
 *
 * @return      none
 */
void flash_WIP_poll(void) {
	unsigned char stat;
	flash_cmd(READ_STAT, 0);
	do {
		stat = spi_rw(0xFF);
	} while ((stat & 0x01) || stat == 0xFF);
	DESELECT();
}

/*********************************************************************
 * @fn          flash_read
 *
 * @brief      	read flash data
 *
 * @param       address: 	where to start read data out
 * 							p:				where to save read data
 * 							cnt:			number of byte want to read
 *
 * @return      none:
 */
void flash_read(unsigned long address, unsigned char* p, unsigned int cnt) {
	flash_WIP_poll();
	flash_cmd(READ_DATA, address);
	do {
		*p++ = spi_rw(0xFF);
	} while (--cnt);
	DESELECT();
}

/*********************************************************************
 * @fn          flash_block_erase
 *
 * @brief      	erase one block data - 64KB
 *
 * @param       address: 	block address to be earsed
 *
 * @return      none:
 */
void flash_block_erase(unsigned long address) {
	flash_WIP_poll();
	flash_cmd(WRITE_ENABLE, 0);
	flash_cmd(BLOCK_ERASE, address);
}

/*********************************************************************
 * @fn          flash_sector_erase
 *
 * @brief      	erase one sector data - 4KB
 *
 * @param       address: 	sector address to be earsed
 *
 * @return      none:
 */
void flash_sector_erase(unsigned long address) {
	flash_WIP_poll();
	flash_cmd(WRITE_ENABLE, 0);
	flash_cmd(SECTOR_ERASE, address);
}

/*********************************************************************
 * @fn          flash_erase
 *
 * @brief      	erase entire flash
 *
 * @param       none
 *
 * @return      none:
 */
void flash_erase() {
	flash_WIP_poll();
	flash_cmd(WRITE_ENABLE, 0);
	flash_cmd(CHIP_ERASE, 0);
}

/*********************************************************************
 * @fn          flash_page_program
 *
 * @brief      	program flash, limit inside 1 page
 *
 * @param       address: 	where to start write data
 * 							p:				data in array
 * 							cnt:			number of byte want to write
 *
 * @return      none:
 */
void flash_page_program(unsigned long address, unsigned char* p,
		unsigned int cnt) {
	unsigned char temp;
	flash_WIP_poll();
	flash_cmd(WRITE_ENABLE, 0);
	flash_cmd(PAGE_PROGRAM, address);
	do {
		temp = spi_rw(*p++);
	} while (--cnt);
	DESELECT();
}

/*********************************************************************
 * @fn          flash_read_id
 *
 * @brief      	read device ID
 *
 * @param       p: 	buffer to read device id out
 *
 * @return      none:
 */
void flash_read_id(unsigned char *p) {
	unsigned char cnt = 4;
	flash_WIP_poll();
	flash_cmd(READ_ID, 0);
	do {
		*p++ = spi_rw(0xFF);
	} while (--cnt);
	DESELECT();
}
void TestFlash(void) {
	unsigned char RdData[64] = { 0 };
	unsigned char data[64] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
			19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
	flash_sector_erase(0);
	flash_page_program(0, data, 64);
	flash_read(0, RdData, 64);
	__delay_ms(100);
}
