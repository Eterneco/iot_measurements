################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
source/hardware/watchdog/watchdog.obj: ../source/hardware/watchdog/watchdog.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"c:/ti/ccsv6/tools/compiler/arm_5.1.6/bin/armcl" -mv7M4 --code_state=16 --float_support=fpalib --abi=eabi -me --include_path="c:/ti/ccsv6/tools/compiler/arm_5.1.6/include" --include_path="C:/ti/CC3200SDK_1.0.0/cc3200-sdk/inc" --include_path="C:/ti/CC3200SDK_1.0.0/cc3200-sdk/example/common" --include_path="C:/ti/CC3200SDK_1.0.0/cc3200-sdk/driverlib" --include_path="C:/ti/CC3200SDK_1.0.0/cc3200-sdk/simplelink" --include_path="C:/ti/CC3200SDK_1.0.0/cc3200-sdk/simplelink/include" --include_path="C:/ti/CC3200SDK_1.0.0/cc3200-sdk/simplelink/source" --include_path="C:/ti/CC3200SDK_1.0.0/cc3200-sdk/oslib/" --include_path="C:/ti/ep/iot_measurements/source/" --include_path="C:/ti/ep/iot_measurements/source/device" --include_path="C:/ti/ep/iot_measurements/source/device/mcp9700" --include_path="C:/ti/ep/iot_measurements/source/hardware" --include_path="C:/ti/ep/iot_measurements/source/hardware/adc" --include_path="C:/ti/ep/iot_measurements/source/hardware/uart" --include_path="C:/ti/ep/iot_measurements/source/hardware/watchdog" --include_path="C:/ti/ep/iot_measurements/source/system" -g --define=ccs --define=cc3200 --define=USE_FREERTOS --define=SL_PLATFORM_MULTI_THREADED --undefine=WATCHDOG_ON --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="source/hardware/watchdog/watchdog.pp" --obj_directory="source/hardware/watchdog" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


