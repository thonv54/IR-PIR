/*
 * I2C.c
 *
 *  Created on: Nov 11, 2015
 *      Author: hp8460p
 */

#include "Device.h"
#include "I2C.h"
#include "delay.h"

typedef union {
	struct {
		unsigned int R :12;
		unsigned char E :4;
	} b;
	unsigned int all :16;
} LightData;

typedef union {
	struct {
		byte OVF :1;
		byte M :2;
		byte CT :1;
		byte RN :4;

		byte FC :2;
		byte ME :1;
		byte POL :1;
		byte L :1;
		byte FL :1;
		byte FH :1;
		byte CRF :1;

	} b;
	unsigned int all :16;
} ConfigData;

void I2C_Delay(unsigned int time) {
	__delay_us(time);
}
void i2c_init(void) {
	SDA_OUTPUT
	SCL = 1;
	I2C_Delay(I2C_DELAY);
	SDA = 1;
	I2C_Delay(I2C_DELAY);
}
void I2C_Start(void) {
	SDA_OUTPUT
	SDA = 1;
	SCL = 1;
	I2C_Delay(I2C_DELAY);
	SDA = 0;
	I2C_Delay(I2C_DELAY);
	SCL = 0;
	I2C_Delay(I2C_DELAY);
}
void I2C_Stop(void) {
	SDA_OUTPUT
	SDA = 0;
	SCL = 1;
	I2C_Delay(I2C_DELAY);
	SDA = 1;
	I2C_Delay(I2C_DELAY);
	SCL = 0;
	I2C_Delay(I2C_DELAY);
}
unsigned char i2c_checkack(void) {
	unsigned char ack = 0;
//	SDA_OUTPUT
//	SDA = 1;
	SDA_INPUT
	SCL = 1;
	I2C_Delay(I2C_DELAY);
	ack = SDA_CHECK_PIN;
	I2C_Delay(I2C_DELAY);
	SCL = 0;
	I2C_Delay(I2C_DELAY);
	if (ack == 1)
		return 0;
	return 1;
}
void i2c_MstAck(void) {
	SDA_OUTPUT
	;
	SDA = 0;
	SCL = 1;
	I2C_Delay(I2C_DELAY);
	SCL = 0;
	I2C_Delay(I2C_DELAY);
}
void i2c_write(unsigned char Data) {
	unsigned char i;
	SDA_OUTPUT
	for (i = 0; i < 8; i++) {
		if ((Data << i) & 0x80)
			SDA_SET_PIN
		else
			SDA_CLR_PIN
		SCL = 1;
		I2C_Delay(I2C_DELAY);
		SCL = 0;
		I2C_Delay(I2C_DELAY);
	}
}
unsigned char i2c_read(void) {
	unsigned char I2C_data = 0, i;
	for (i = 0; i < 8; i++) {
//		SDA_OUTPUT
//		SDA = 1;
		I2C_data = I2C_data << 1;
		SCL = 1;
		SDA_INPUT
		I2C_Delay(I2C_DELAY);
		if (SDA_CHECK_PIN)
			I2C_data = I2C_data | 0x01;
		SCL = 0;
		I2C_Delay(I2C_DELAY);
	}
	return I2C_data;
}

unsigned int i2c_SI7021_get(unsigned char addr) {
	unsigned int ret = 0;
	unsigned char AckTemp = 1;
	I2C_Start(); /* Start i2c bus */

	i2c_write(SI7021_ADDR); /* Connect to OPT3001 */
	AckTemp = i2c_checkack();

	i2c_write(addr); /* Request RAM address on OPT3001 */
	AckTemp = i2c_checkack();

	I2C_Start(); /* Start i2c bus */

	i2c_write(SI7021_ADDR + 1); /* Connect to OPT3001 for Read */
	AckTemp = i2c_checkack();
	__delay_ms(50);
	// SCL may be high. set SCL low. If SDA goes high when input
	SCL = 0;
	SDA = 1;

	ret = i2c_read(); /* Receive data */
	i2c_MstAck();
	__delay_us(100);
	ret = ret << 8 | i2c_read();
	AckTemp = i2c_checkack();
	I2C_Stop();
//*********************************************
//   temp = ret;    /*BCD to HEX*/
//   ret = (unsigned char)(((ret/16)*10)+ (temp & 0x0f));    /*for Led 7seg*/
//*********************************************   
	return ret;
}

void i2c_SI7021_Write(unsigned char addr, unsigned char dat) {

	unsigned char AckTemp = 1;
//**********************************************    /*HEX to BCD*/
//   temp = dat ;    /*for Led 7seg*/
//   dat = (((dat/10)*16)|(temp %10));
//**********************************************   
	I2C_Start(); /* Start i2c bus */
	
	i2c_write(SI7021_ADDR); /* Connect to OPT3001 */
	
	AckTemp = i2c_checkack();
	i2c_write(addr); /* Request RAM address on OPT3001 */
	AckTemp = i2c_checkack();
	i2c_write(dat); /* Connect to OPT3001 for Read */
	AckTemp = i2c_checkack();
	I2C_Stop();
}

void i2c_SI7021_Config(void) {
	i2c_SI7021_Write(ConfigSI, 0x3B);
	__delay_ms(100);
}

unsigned char i2c_SI7021_ReadFirmwave(void){
	unsigned char SiFirmwave = 0;
	unsigned char AckTemp = 1;
	I2C_Start(); /* Start i2c bus */

	i2c_write(SI7021_ADDR); /* Connect to OPT3001 */
	AckTemp = i2c_checkack();

	i2c_write(SiFwRegHi); /* Request RAM address on OPT3001 */
	AckTemp = i2c_checkack();

	i2c_write(SiFwRegLow); /* Request RAM address on OPT3001 */
	AckTemp = i2c_checkack();
		
	I2C_Start(); /* Start i2c bus */

	i2c_write(SI7021_ADDR + 1); /* Connect to OPT3001 for Read */
	AckTemp = i2c_checkack();
	

	// SCL may be high. set SCL low. If SDA goes high when input
	SCL = 0;
	SDA = 1;

	SiFirmwave = i2c_read(); /* Receive data */
	i2c_MstAck();
	AckTemp = i2c_checkack();
	I2C_Stop();
//*********************************************
//   temp = ret;    /*BCD to HEX*/
//   ret = (unsigned char)(((ret/16)*10)+ (temp & 0x0f));    /*for Led 7seg*/
//*********************************************   
	return SiFirmwave;
}


unsigned int i2c_OPT3001_ReadId(void){
	unsigned int tempID;
	tempID = i2c_OPT3001_get(DID);
	return tempID;
	
}

unsigned int i2c_OPT3001_get(unsigned char addr) {
	unsigned int temp;
	unsigned int ret = 0;
	unsigned char AckTemp = 1;
	I2C_Start(); /* Start i2c bus */

	i2c_write(OPT3001_ADDR); /* Connect to OPT3001 */
	AckTemp = i2c_checkack();

	i2c_write(addr); /* Request RAM address on OPT3001 */
	AckTemp = i2c_checkack();

	I2C_Start(); /* Start i2c bus */

	i2c_write(OPT3001_ADDR + 1); /* Connect to OPT3001 for Read */
	AckTemp = i2c_checkack();

	// SCL may be high. set SCL low. If SDA goes high when input
	SCL = 0;
	SDA = 1;

	ret = i2c_read(); /* Receive data */
	i2c_MstAck();
	__delay_us(100);
	ret = ret << 8 | i2c_read();
	AckTemp = i2c_checkack();
	I2C_Stop();
//*********************************************
//   temp = ret;    /*BCD to HEX*/
//   ret = (unsigned char)(((ret/16)*10)+ (temp & 0x0f));    /*for Led 7seg*/
//*********************************************   
	return ret;
}

void i2c_OPT3001_Write(unsigned char addr, unsigned int dat) {
	unsigned int temp;
	unsigned char AckTemp = 1;
//**********************************************    /*HEX to BCD*/
//   temp = dat ;    /*for Led 7seg*/
//   dat = (((dat/10)*16)|(temp %10));
//**********************************************   
	I2C_Start(); /* Start i2c bus */
	i2c_write(OPT3001_ADDR); /* Connect to OPT3001 */
	AckTemp = i2c_checkack();
	i2c_write(addr); /* Request RAM address on OPT3001 */
	AckTemp = i2c_checkack();
	i2c_write((unsigned char) (dat >> 8 & 0xFF)); /* Connect to OPT3001 for Read */
	AckTemp = i2c_checkack();
	i2c_write((unsigned char) (dat)); /* Connect to OPT3001 for Read */
	AckTemp = i2c_checkack();
	I2C_Stop();
}

void i2c_OPT3011_Config(void) {
	i2c_OPT3001_Write(ConfigOPT, 0xC200);
	__delay_ms(100);
}

unsigned long shift(unsigned long data, unsigned int n) {
	unsigned int i;
	for (i = 0; i < n; i++) {
		data = data << 1;
	}
	return data;
}

unsigned int I2C_ReadLightValue(void) {
	LightData tempLightData;
	ConfigData tempConfigData;
	unsigned long cnt = 0;
	unsigned char i;
	unsigned int dataE = 0;
	unsigned long dataR = 0;
	unsigned long templux[2] = { 0 };
	unsigned int lux;
	for (i = 0; i < 2; i++) {
		i2c_OPT3001_Write(ConfigOPT, 0xC200);
		do {
			tempConfigData.all = i2c_OPT3001_get(ConfigOPT);
			cnt++;
		} while ((tempConfigData.b.CRF == 1) && (cnt < 10));
		if(cnt == 10){
			return 0;
		}
		tempLightData.all = i2c_OPT3001_get(Result);
		dataE = tempLightData.b.E;
		dataR = tempLightData.b.R;
		templux[i] = shift(dataR, dataE) / 100;
		__delay_ms(100);
	}
	if (abs2int(templux[0], templux[1]) < 20) {
		lux = (unsigned int) ((templux[0] + templux[1]) >> 1);
		return lux;
	}
	return max((unsigned int)templux[0], (unsigned int)templux[1]);
}

