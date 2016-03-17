/*
 * PIR.c
 *
 *  Created on: Nov 11, 2015
 *      Author: hp8460p
 */

#include "PIR.h"
#include "Device.h"
#include "CC.h"
#include "I2C.h"
#include "ADC.h"
#include "RelayControl.h"
#include "RTC.h"
#include "Init_IO.h"
#include "UART.h"
#include "wdog.h"
#include "FTM.h"



#define CurrentPIRStatus 	PIR_IN
#define MaxNumberOfTimeOutInScale 	14

unsigned char RlStatusFlag;
unsigned int Pir_Timeout = 0;

unsigned int TimeOutInScale[MaxNumberOfTimeOutInScale] = { 0, 50, 300, 600, 900, 1200, 1800, 2400,
		3000, 4500, 6000, 9000, 12000, 18000};

unsigned int StepADC[14]	=	StepADCTimeArray;
unsigned char ResetFlag = 0;
unsigned char Pir_status = Pir_off;

extern unsigned char CC_ready_flag;

unsigned int TimeBefore = 0;

unsigned int StartTime = 0;
unsigned int LastTime = 0;
unsigned char DetectStatus = 0;
unsigned char LastPIRStatus = Off;

unsigned char FirstLoadFlag = 0;
unsigned char Off_RTC_cnt = 0;
static unsigned int FTM2_P_CNT = 0;
static unsigned long ResetCnt = 0;

unsigned char ResetStep = 0;

unsigned int mtim_cnt = 0;


unsigned int CalcTimeOut(void) {
	unsigned int ADC_Var = 0;
	unsigned int tempTimeOut;
	unsigned char i;
	ADC_Var = ADC_GetTimeVar;
	if (ADC_Var > MaxTimeADCValue) {
		ADC_Var = MaxTimeADCValue;
	}
	ADC_Var = (unsigned int)(MaxTimeADCValue - ADC_Var);
	//	ADC_Var = MaxTimeADCValue - ADC_Var;
	for (i = 0; i < 13; i++) {
		if (((MaxTimeADCValue / MaxStepADCTimeArray) * StepADC[i] <= ADC_Var)
				&& (ADC_Var
						< (MaxTimeADCValue / MaxStepADCTimeArray)
						* StepADC[i + 1])) {
			break;
		}
	}
	tempTimeOut = TimeOutInScale[i];
	//	ADC_Var = testTimeVar;
	return tempTimeOut;
}

unsigned int CalcLightVar(void){
	unsigned int ADC_Var;
	unsigned int tempLightVar;
	ADC_Var = ADC_GetLightVar;
	//	ADC_Var = testLightVar;
	if (ADC_Var > MaxLightADCValue) {
		ADC_Var = MaxLightADCValue;
	}
	ADC_Var = (unsigned int)(MaxLightADCValue - ADC_Var);
	tempLightVar = (unsigned int)((MaxLightSensor - MinLightSensor) * (ADC_Var - MinLightADCValue)
			/ (MaxLightADCValue - MinLightADCValue) + MinLightSensor);
	if (tempLightVar > FullLightSensor){
		tempLightVar = 0xFFFF;
	}
	return tempLightVar;
}

//#define DebugData

interrupt VectorNumber_Vftm2ovf void FTM2_ISR(void) {
	FTM2_SC_TOF = 0;
}
interrupt VectorNumber_Vftm0ovf void FTMO_ISR(void) {
	FTM0_SC_TOF = 0;
	if(ResetStep !=0){
		ResetCnt ++;
	}
	if(ResetCnt == 20){
		ResetCnt = 0;
		ResetStep = 0;
	}

}
void ScanPIR(void){
	unsigned int templightdata = 0;
	unsigned int templightvar = 0;
	unsigned int temptimevar = 0;

	if((CurrentPIRStatus == Off) && (LastPIRStatus == On)){
		if(FirstLoadFlag == 0){
			FirstLoadFlag = 1;
		}
		else{
			//kiem tra thoi gian xung tat
			ResetFTMTimer();
			while(CurrentPIRStatus == Off){
				if(FTM2_CNT > 1000){
					break;
				}
			}
			if(FTM2_CNT > 1000){
				StatusLedOn;
				if(Pir_status == Pir_off){
					Pir_status = Pir_on;
					CC_F0_PirSendData();
				}
							#ifdef DebugData 	
								temptimevar   = ADC_GetTimeVar ;
								templightvar  = ADC_GetLightVar;	
								templightdata = I2C_ReadLightValue();
								UART0_puts("light var:");
								UART0_puti(templightvar);
								UART0_puts("  ");
								UART0_puts("light data:");
								UART0_puti(templightdata);
								UART0_puts("\n\r");
								UART0_puts("time var:");
								UART0_puti(temptimevar); 
								UART0_puts("\n\r");
							#endif
				Pir_Timeout = CalcTimeOut();
#ifdef PIR_CHONGTROM
				{
					if((Pir_Timeout == 0) && (ResetStep == 0)){
						ResetStep = 1;
						ResetCnt = 0;
					}
					else if((Pir_Timeout == TimeOutInScale[MaxNumberOfTimeOutInScale]) && (ResetStep == 1)){
						ResetStep = 2;
					}
					else if((Pir_Timeout == 0) && (ResetStep == 2)){
						ResetStep = 3;
					}
					else if((Pir_Timeout == TimeOutInScale[MaxNumberOfTimeOutInScale]) && (ResetStep == 3)){
						ResetStep = 4;
						BlinkLedDetect;
					}
				}
#endif
				if (RlStatusFlag == On){
					__delay_ms(50);
				}
				else{
					//check light
					templightvar  = CalcLightVar();
						
					templightdata = I2C_ReadLightValue();
					if(templightdata < templightvar){
						if(Pir_Timeout > 12 ){
							RL_on();
						}
					}
				}
				EnableRTCTimer;
				ResetRTCTimer();
				StatusLedOff;
			}
		}
	}
	LastPIRStatus = CurrentPIRStatus;
}

void PIR_Program(void){
	unsigned int LightSensorValue = 0;
	unsigned int TimeOutVar = 0;
	//check time
	if(Pir_status == Pir_on || RlStatusFlag == On){
		if (RTC_CNT > Pir_Timeout){
				//if rl == on check for off
				if(RlStatusFlag == On){
					//rl off
					RL_off();
					DisableRTCTimer;
				//send CCcmd
				}
				if(Pir_status == Pir_on){
					Pir_status = Pir_off;
					CC_F0_PirSendData();
				
			}
		}
	}
	// neu khong co nguoi 
	//wait CCCmd
	CC_receiver_program();
}

void PirStartUp(void){
	unsigned char CheckCCReadyCnt = 0;
	RL_off();
	StatusLedOn;
	RlStatusFlag = Off;
	//	BlueLedOn;
	GetPwSource();
	DisableRTCTimer;
	i2c_OPT3011_Config();
	if(i2c_OPT3001_ReadId() != 0x3001){
		RedLedBlinkLongTime;
	}
#ifdef CC_Interface
	while (CC_ready_flag == 0){
		CheckCCReadyCnt ++;	
		CC_send_SynClock();
		__delay_ms(100);
		CC_receiver_program();
		if(CheckCCReadyCnt == 20){
			RedLedBlinkLongTime;
			RedLedBlinkLongTime;
			Reset_MCU();
		}
	}
#endif
}

interrupt VectorNumber_Vmtim0 void MTIMO_ISR(void) {
	unsigned char i;
	unsigned int temp = 0;
	if (MTIM0_SC_TOF) {
		mtim_cnt ++;
		if(mtim_cnt == 20){
			mtim_cnt = 0;
			UART0_putchar(0x0D);		
			UART0_putchar(0x00);
			if(PIR_IN == 1){
				temp = 0;
				UART0_putchar((unsigned char)(temp>>8));
				UART0_putchar((unsigned char)temp);
			}
			else{
				temp = 2;
				UART0_putchar((unsigned char)(temp>>8));
				UART0_putchar((unsigned char)temp);
			}
			for (i=0; i<3; i++){
				UART0_putchar(0x00);
				temp = 1;
				UART0_putchar((unsigned char)(temp>>8));
				UART0_putchar((unsigned char)temp);
			}
		}
	}
	MTIM0_SC_TOF = 0;
}
