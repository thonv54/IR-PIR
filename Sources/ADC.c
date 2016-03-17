/*
 * ADC.c
 *
 *  Created on: Oct 25, 2014
 *      Author: hp8460p
 */
#include "ADC.h"
#include "Device.h"



void Init_ADC( void )
{
	ADC_APCTL1_ADPC4 = 0;
	ADC_APCTL2_ADPC9 = 0;
	ADC_APCTL2_ADPC10 = 0;

	ADC_SC1 = 0;
	ADC_SC2 |= 0x01;
	ADC_SC3_ADICLK = CLOCK_SOURCE_BUS_CLOCK_DIVIDE_2	;
	ADC_SC3_ADIV   = ADC_ADIV_DIVIDE_4;
	ADC_SC3_MODE   = ADC_MODE_12BIT;
	ADC_SC3_ADLSMP = 1;
	
}
/*****************************************************************************//*!
   +FUNCTION----------------------------------------------------------------
   * @function name: ADC_GetResult
   *
   * @brief start a conversion and get conversion result
   *        
   * @param  none 
   *
   * @return none
   *
   * @ Pass/ Fail criteria: none
   *****************************************************************************/
unsigned short ADC_GetResult( unsigned char channel)
{
	unsigned char i;
	unsigned short temp=0;
	for(i=0;i<3;i++){
		ADC_SC1 = channel; // initial the ADC conversion,select Vrefh
		while( !ADC_SC1_COCO );
		if(i==0)
		{
			temp = ADC_R;
		}
		else
		{
			temp = (temp+ADC_R)>>1;
		}
	}
	return temp;
}

void ADCWaitZeroPoint(unsigned char channel){
	unsigned char LowTrue_cnt = 0;
	unsigned char HightTrue_cnt = 0;
	unsigned char i = 0;

	while (HightTrue_cnt<3){
		if(ADC_GetResult(channel)>3500){				// dinh ADC=4095
			HightTrue_cnt++;
		}
		else{
			HightTrue_cnt = 0;
		}
	while(LowTrue_cnt<3){
		if(ADC_GetResult(channel)<150){
			LowTrue_cnt++;
		}
		else{
			LowTrue_cnt=0;
		}
	}		
	ADC_SC1=0x1F;
	}
	
}

