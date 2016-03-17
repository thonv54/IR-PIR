/*
 * I2C.h
 *
 *  Created on: Nov 11, 2015
 *      Author: hp8460p
 */

#ifndef I2C_H_
#define I2C_H_
#include "Device.h"

#define I2C_DELAY      10
//#define SCL_PORT	GPIOB
//#define SDA_PORT	GPIOB

// * 	PB6:		SDA				Output
// * 	PB7:		SCL				Output
#define SDA		PORT_PTBD_PTBD6
#define SCL		PORT_PTBD_PTBD7


#define	SDA_OUTPUT				{PORT_PTBIE_PTBIE6 = 0; PORT_PTBOE_PTBOE6 =	1; }
#define	SDA_INPUT				{PORT_PTBIE_PTBIE6 = 1; PORT_PTBOE_PTBOE6 =	0; }

#define	SCL_SET_PIN				{ SCL = 1; }
#define	SCL_CLR_PIN				{ SCL = 0; }
#define	SDA_SET_PIN				{ SDA = 1; }
#define	SDA_CLR_PIN				{ SDA = 0; }
#define	SDA_CHECK_PIN			((PORT_PTBD_PTBD6 == 1) ? 1 : 0)

#define SI7021_ADDR 0x80
#define	OPT3001_ADDR 0x88


#define ConfigSI	0xE6
#define SiFwRegHi	0x84
#define SiFwRegLow	0xB8
#define HumiditySiReg 0xE5
#define TemperatureSiReg 0xE3

#define Result	0x00
#define ConfigOPT	0x01
#define LowLimit	0x02
#define HighLimit	0x03
#define MID			0x7E
#define DID			0x7F


//#define delay_time_us  1

void i2c_init(void);
void I2C_Start(void);
void I2C_Stop(void);
unsigned char i2c_checkack(void);
void i2c_write(unsigned char Data);
unsigned char i2c_read(void);
unsigned int i2c_OPT3001_get(unsigned char addr);
void i2c_OPT3001_Write(unsigned char addr, unsigned int dat);
void i2c_OPT3011_Config(void);
unsigned int I2C_ReadLightValue(void);
unsigned int i2c_OPT3001_ReadId(void);


unsigned char i2c_SI7021_ReadFirmwave(void);
void i2c_SI7021_Config(void);
unsigned int i2c_SI7021_get(unsigned char addr);

#endif /* I2C_H_ */
