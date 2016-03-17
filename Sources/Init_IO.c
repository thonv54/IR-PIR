/*
 * IO.c
 *
 *  Created on: Sep 30, 2014
 *      Author: hp8460p
 */

#include "Init_IO.h"
#include "EEP_ROM.h"


/************************************************************************************
 MC9S08PT16 IO library
 LUMI Viet Nam
 Write by: ThoNV
 ************************************************************************************
 * 
 ********************************************************************************** */
/*
 * ------------------------------------------------------------------------
 */
//#define EM357
/*
 * FN: Init_IO
 * config cac chan IO cua chip tuong ung
 * Tham so dau vao : khong
 * Gia tri tra ve :	khong
 * 
 * ----Cau hinh port A------		
 * 
 * 	PA0:		PIR_IN			Output
 *  PA1:		IR_IN			Input
 *  PA2:		RX_MC			Input
 *  PA3:		TX_MC			Output
 * 	PA4: 		BKGD
 * 	PA5: 		RESET
 * 	PA6:		IR_OUT			Output
 * 	PA7:		Not connect		Output
 * 	
 * ----Cau hinh port B------
 * 
 * 	PB0: 		ZERO_DT_PW		Input Analog	
 * 	PB1:		Not connect		Output
 * 	PB2:		SPI_CLK			Output
 * 	PB3:		SPI_MOSI		Output
 * 	PB4:		SPI_MISO		Input
 * 	PB5:		SPI_CS			Output
 * 	PB6:		SDA				Output
 * 	PB7:		SCL				Output
 * 
 * 	----Cau hinh port C------
 * 	
 * 	PC0:		Not connect		Output 
 * 	PC1:		VR_Light		Input Analog
 * 	PC2:		VR_Timer		Input Analog
 * 	PC3:		Not connect		Output	
 * 	PC4:		Select			Output
 * 	PC5:		RL1				Output
 * 	PC6: 		CT_PW_CC		Output
 * 	PC7:		BUTTON			Input
 * 	
 * 	----Cau hinh port D------
 * 	
 * 	PD0: 		Led_blue 			Output
 * 	PD1:		Led_red 			Output 
 * 	PD2:		Not Connect			Output
 * 	PD3:		Not Connect			Output
 * 	PD4: 
 * 	PD5: 
 * 	PD6: 
 * 	PD7:	
 */
void Init_IO(void) {


	PORT_PTAIE = 0x00 | PORT_PTAIE_PTAIE1_MASK | PORT_PTAIE_PTAIE0_MASK;
	PORT_PTAOE = ~PORT_PTAIE;
	PORT_PTAPE = 0x00 | PORT_PTAPE_PTAPE1_MASK | PORT_PTAPE_PTAPE0_MASK;
	PORT_PTAD = 0x00;

	PORT_PTBIE = 0x00 | PORT_PTBIE_PTBIE0_MASK | PORT_PTBIE_PTBIE4_MASK;
	PORT_PTBOE = ~PORT_PTBIE;
	PORT_PTBPE = PORT_PTBPE | PORT_PTBPE_PTBPE4_MASK;
	PORT_PTBPE = 0x00;
	PORT_PTBD = 0x00;

	PORT_PTCIE = 0x00 | PORT_PTCIE_PTCIE1_MASK | PORT_PTCIE_PTCIE2_MASK | PORT_PTCIE_PTCIE7_MASK;
	PORT_PTCOE = ~PORT_PTCIE;
	PORT_PTCPE = 0x00;
	PORT_PTCD = 0x00;

	PORT_PTDIE = 0x00;
	PORT_PTDOE = ~PORT_PTDIE;
	PORT_PTDPE = 0x00;
	PORT_PTDD = 0x00;

}
/* Fn: clock_config
 * cau hinh bo dao dong noi cua chip
 * Tham so dau vao: khong
 * Gia tri tra ve : khong
 * 
 */
void clock_config(void) {
	unsigned char TRIM_store;
	TRIM_store = FLASH_Read(0xFF6E);
	ICS_C2 = 0x00;
	ICS_C1 = 0x04; /* internal reference clock to FLL */

	if ((0x20 < TRIM_store) & (TRIM_store < 0x70)) {
		ICS_C3 = TRIM_store;
	}

	ICS_C2_BDIV = 0b000; /*1: 2 prescalar */ // bus_clock = 10Mhz
	//0: no presscaler // bus_clock = 20Mhz	
}
/* Fn: system_config
 * cau hinh cac modun cua chip, che do hoat dong, cho phep cac modun su dung cac chan IO thich hop
 * Tham so dau vao: khong
 * Gia tri tra ve : khong
 */
void system_config(void) {
	SYS_SRS = 0x00; // POR on, watch dog off,pin reset enable, low voltage detect off
	//	SYS_SOPT1 = 0x08;    // reset not by pin PTA5, stop mode disabled, CPU wake up normal
	//	SYS_SOPT3  = 0x08;   // bus clock in PTE3 off
	SYS_SOPT1 = 0x8D;
	PMC_SPMSC1_LVDRE = 1;			// low voltage reset
	PMC_SPMSC2_LVDV = 0;
	SYS_SRS_POR = SYS_SRS_POR_MASK | SYS_SRS_LVD_MASK;				// POR reset

}


/*
 * FN :load_detect
 * Tham so dau vao:khong
 * Gia tri tra ve : khong
 * -> kiem tra loai cua tai va kiem tra nguon cap, dua vao cac chan ADC feedback
 * 
 * -> kiem tra lien tuc 100 lan cac chan ADC feedback trong vong 20ms -> neu phat hien co xung thay doi gia tri, nghia la co 
 * nguon ( voi chan ADC feedback Power) co tro tai ( voi cac chan ADC feedback role) 
 */







