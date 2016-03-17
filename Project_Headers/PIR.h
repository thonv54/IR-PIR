/*
 * PIR.h
 *
 *  Created on: Nov 11, 2015
 *      Author: hp8460p
 */

#ifndef PIR_H_
#define PIR_H_

#define THIS_TIME		(FTM2_P_CNT * 65535UL + FTM2_CNT)

#define PIR_IN		PORT_PTAD_PTAD0
#define MinTimeADCValue	0UL
#define MaxTimeADCValue 4005UL

#define StepADCTimeArray	{0, 1, 7, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65}
#define MaxStepADCTimeArray	65

#define MinLightADCValue	0UL
#define MaxLightADCValue 4005UL


#define StepADCLightArray	{0 50 100 150}
#define MaxStepADCLightArray	150 

#define MaxLightSensor	400UL			//lux
#define FullLightSensor 380UL
#define MinLightSensor	0UL


#define ADC_GetTimeVar		ADC_GetResult(LightVarChanel)
#define ADC_GetLightVar 	ADC_GetResult(TimeVarChanel)
unsigned int CalcLightVar(void);
unsigned int CalcTimeOut(void);


void PIR_Program(void);
void PirStartUp(void);
void ScanPIR(void);
void TestPir(void);

#endif /* PIR_H_ */





