/*
 * ADC.h
 *
 *  Created on: Oct 25, 2014
 *      Author: hp8460p
 */

#ifndef ADC_H_
#define ADC_H_
#define ADC_STATE_NULL					0x00
#define ADC_STATE_READED_DATA			0x01  // read data from FIFO/result register ,wait to handle

#define ADC_CAPTURE_VALUE               0x100

#define ADC_WORK_MODE_NORMAL							0x00
#define ADC_WORK_MODE_WITH_FIFO							0x01
#define ADC_WORK_MODE_WITHOUT_FIFO						0x02
#define ADC_WORK_MODE_WITHOUT_FIFO_CAPTURE				0x03
#define ADC_WORK_MODE_WITH_FIFO_CAPTURE                 0x04
#define ADC_WORK_MODE_WAIT								0x05
#define ADC_WORK_MODE_STOP								0x06


#define CLOCK_SOURCE_BUS_CLOCK							0x00
#define CLOCK_SOURCE_BUS_CLOCK_DIVIDE_2					0x01
#define CLOCK_SOURCE_ALTCLK								0x02
#define CLOCK_SOURCE_ADACK								0x03

#define ADC_ADIV_DIVIDE_1								0x00
#define ADC_ADIV_DIVIDE_2								0x01
#define ADC_ADIV_DIVIDE_4								0x02
#define ADC_ADIV_DIVIDE_8								0x03

#define ADC_MODE_8BIT									0x00
#define ADC_MODE_10BIT									0x01
#define ADC_MODE_12BIT									0x02


#define LightVarChanel	10
#define TimeVarChanel	9



/******************************************************************************
* Global function
******************************************************************************/
void Init_ADC( void );
unsigned short ADC_GetResult( unsigned char channel);
void ADCWaitZeroPoint(unsigned char channel);


#endif /* ADC_H_ */
