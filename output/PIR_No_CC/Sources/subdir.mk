################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../../Sources/ADC.c" \
"../../Sources/CC.c" \
"../../Sources/EEP_ROM.c" \
"../../Sources/FTM.c" \
"../../Sources/I2C.c" \
"../../Sources/Init_IO.c" \
"../../Sources/KBI.c" \
"../../Sources/MTIM.c" \
"../../Sources/PIR.c" \
"../../Sources/RTC.c" \
"../../Sources/RelayControl.c" \
"../../Sources/UART.c" \
"../../Sources/delay.c" \
"../../Sources/main.c" \
"../../Sources/wdog.c" \

C_SRCS += \
../../Sources/ADC.c \
../../Sources/CC.c \
../../Sources/EEP_ROM.c \
../../Sources/FTM.c \
../../Sources/I2C.c \
../../Sources/Init_IO.c \
../../Sources/KBI.c \
../../Sources/MTIM.c \
../../Sources/PIR.c \
../../Sources/RTC.c \
../../Sources/RelayControl.c \
../../Sources/UART.c \
../../Sources/delay.c \
../../Sources/main.c \
../../Sources/wdog.c \

OBJS += \
./Sources/ADC_c.obj \
./Sources/CC_c.obj \
./Sources/EEP_ROM_c.obj \
./Sources/FTM_c.obj \
./Sources/I2C_c.obj \
./Sources/Init_IO_c.obj \
./Sources/KBI_c.obj \
./Sources/MTIM_c.obj \
./Sources/PIR_c.obj \
./Sources/RTC_c.obj \
./Sources/RelayControl_c.obj \
./Sources/UART_c.obj \
./Sources/delay_c.obj \
./Sources/main_c.obj \
./Sources/wdog_c.obj \

OBJS_QUOTED += \
"./Sources/ADC_c.obj" \
"./Sources/CC_c.obj" \
"./Sources/EEP_ROM_c.obj" \
"./Sources/FTM_c.obj" \
"./Sources/I2C_c.obj" \
"./Sources/Init_IO_c.obj" \
"./Sources/KBI_c.obj" \
"./Sources/MTIM_c.obj" \
"./Sources/PIR_c.obj" \
"./Sources/RTC_c.obj" \
"./Sources/RelayControl_c.obj" \
"./Sources/UART_c.obj" \
"./Sources/delay_c.obj" \
"./Sources/main_c.obj" \
"./Sources/wdog_c.obj" \

C_DEPS += \
./Sources/ADC_c.d \
./Sources/CC_c.d \
./Sources/EEP_ROM_c.d \
./Sources/FTM_c.d \
./Sources/I2C_c.d \
./Sources/Init_IO_c.d \
./Sources/KBI_c.d \
./Sources/MTIM_c.d \
./Sources/PIR_c.d \
./Sources/RTC_c.d \
./Sources/RelayControl_c.d \
./Sources/UART_c.d \
./Sources/delay_c.d \
./Sources/main_c.d \
./Sources/wdog_c.d \

C_DEPS_QUOTED += \
"./Sources/ADC_c.d" \
"./Sources/CC_c.d" \
"./Sources/EEP_ROM_c.d" \
"./Sources/FTM_c.d" \
"./Sources/I2C_c.d" \
"./Sources/Init_IO_c.d" \
"./Sources/KBI_c.d" \
"./Sources/MTIM_c.d" \
"./Sources/PIR_c.d" \
"./Sources/RTC_c.d" \
"./Sources/RelayControl_c.d" \
"./Sources/UART_c.d" \
"./Sources/delay_c.d" \
"./Sources/main_c.d" \
"./Sources/wdog_c.d" \

OBJS_OS_FORMAT += \
./Sources/ADC_c.obj \
./Sources/CC_c.obj \
./Sources/EEP_ROM_c.obj \
./Sources/FTM_c.obj \
./Sources/I2C_c.obj \
./Sources/Init_IO_c.obj \
./Sources/KBI_c.obj \
./Sources/MTIM_c.obj \
./Sources/PIR_c.obj \
./Sources/RTC_c.obj \
./Sources/RelayControl_c.obj \
./Sources/UART_c.obj \
./Sources/delay_c.obj \
./Sources/main_c.obj \
./Sources/wdog_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/ADC_c.obj: $(SOURCES)/ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/ADC.args" -ObjN="Sources/ADC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/ADC_c.d: $(SOURCES)/ADC.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/CC_c.obj: $(ProjDirPath)/Sources/CC.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/CC.args" -ObjN="Sources/CC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/CC_c.d: $(ProjDirPath)/Sources/CC.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/EEP_ROM_c.obj: $(ProjDirPath)/Sources/EEP_ROM.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/EEP_ROM.args" -ObjN="Sources/EEP_ROM_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/EEP_ROM_c.d: $(ProjDirPath)/Sources/EEP_ROM.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/FTM_c.obj: $(ProjDirPath)/Sources/FTM.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/FTM.args" -ObjN="Sources/FTM_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/FTM_c.d: $(ProjDirPath)/Sources/FTM.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/I2C_c.obj: $(ProjDirPath)/Sources/I2C.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/I2C.args" -ObjN="Sources/I2C_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/I2C_c.d: $(ProjDirPath)/Sources/I2C.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/Init_IO_c.obj: $(ProjDirPath)/Sources/Init_IO.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/Init_IO.args" -ObjN="Sources/Init_IO_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/Init_IO_c.d: $(ProjDirPath)/Sources/Init_IO.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/KBI_c.obj: $(ProjDirPath)/Sources/KBI.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/KBI.args" -ObjN="Sources/KBI_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/KBI_c.d: $(ProjDirPath)/Sources/KBI.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/MTIM_c.obj: $(ProjDirPath)/Sources/MTIM.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/MTIM.args" -ObjN="Sources/MTIM_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/MTIM_c.d: $(ProjDirPath)/Sources/MTIM.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/PIR_c.obj: $(ProjDirPath)/Sources/PIR.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/PIR.args" -ObjN="Sources/PIR_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/PIR_c.d: $(ProjDirPath)/Sources/PIR.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/RTC_c.obj: $(ProjDirPath)/Sources/RTC.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/RTC.args" -ObjN="Sources/RTC_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/RTC_c.d: $(ProjDirPath)/Sources/RTC.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/RelayControl_c.obj: $(ProjDirPath)/Sources/RelayControl.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/RelayControl.args" -ObjN="Sources/RelayControl_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/RelayControl_c.d: $(ProjDirPath)/Sources/RelayControl.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/UART_c.obj: $(ProjDirPath)/Sources/UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/UART.args" -ObjN="Sources/UART_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/UART_c.d: $(ProjDirPath)/Sources/UART.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/delay_c.obj: $(ProjDirPath)/Sources/delay.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/delay.args" -ObjN="Sources/delay_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/delay_c.d: $(ProjDirPath)/Sources/delay.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/main_c.obj: $(ProjDirPath)/Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/main_c.d: $(ProjDirPath)/Sources/main.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/wdog_c.obj: $(ProjDirPath)/Sources/wdog.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/wdog.args" -ObjN="Sources/wdog_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/wdog_c.d: $(ProjDirPath)/Sources/wdog.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


