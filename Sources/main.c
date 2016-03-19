#include "Device.h"
#include "Init_IO.h"
#include "wdog.h"
#include "KBI.h"
#include "UART.h"
#include "CC.h"
#include "RTC.h"
#include "I2C.h"

#ifdef IR
#include "IR.h"
#include "SPI.h"
#include "FTM.h"
#include "MTIM.h"
#endif

#ifdef PIR
#include "PIR.h"
#include "FTM.h"
#include "ADC.h"
#endif

unsigned int HumidityData = 0;
unsigned int TemperatureData = 0;
PirStatus_t tempstatus;
//void testVar(void){
//	unsigned int light = 0;
//	unsigned int time = 0;
//	light = ADC_GetLightVar;
//	time = ADC_GetTimeVar;
//	UART0_puti(light);
//	UART0_puti(time);
//	__delay_ms(1000);
//}
//void testSI(void) {
//	unsigned int data = 0;
//	unsigned long CelciusTemperature = 0;
//	unsigned long RelativeHumidity = 0;
//	data = (unsigned int) i2c_SI7021_ReadFirmwave();
//	UART0_puti(data);
//	UART0_puts(" ");
//	__delay_ms(500);
//	i2c_SI7021_Config();
//	__delay_ms(500);
//	HumidityData = i2c_SI7021_get(HumiditySiReg);
//	RelativeHumidity = (125UL*HumidityData*100/65536 - 600);
//	UART0_puti((unsigned int)RelativeHumidity);
//	UART0_puts("  ");
//	__delay_ms(500);
//	TemperatureData = i2c_SI7021_get(TemperatureSiReg);
//	CelciusTemperature = ((17572UL * TemperatureData) / 65536 - 4685);
//	UART0_puti((unsigned int) CelciusTemperature);
//	UART0_puts("  ");
//	UART0_puts("\n\r");
//	__delay_ms(500);
//}
void main(void) {

	DisableInterrupts;
	wdog_disable();
	EnableInterrupts;
	clock_config();
	system_config();
	Init_IO();
	AllLedBlink;
	Init_RTC();
	
#ifdef IR
	Init_UART0(19200);
	Init_FLEX_TIMER();
	SPI0_Init();
	Init_MTIM0();
	IR_OFF
	;
	IrStartUp();
	Init_KBI();
#endif
#ifdef PIR
	#ifdef CC_Interface
		Init_UART0(19200);
	#endif
	#if 0
		Init_MTIM0();
	#endif
	Init_FLEX_TIMER();
	Init_ADC();
	PirStartUp();
#endif
	
#ifndef Debug
	wdog_enable();
	wdog_reconfiguring(10000, 1);		//soucer clock = 1kHz, T=8s
	wdog_refreshing();
#endif
	while (1) {
#ifdef IR
		IR_Program();
#endif	
#ifdef PIR
		PIR_Program();
		ScanPIR();
#endif
		
#ifndef Debug
		if (WDOG_CNT > 3500) {
			wdog_refreshing();
		}
#endif
	}
}

