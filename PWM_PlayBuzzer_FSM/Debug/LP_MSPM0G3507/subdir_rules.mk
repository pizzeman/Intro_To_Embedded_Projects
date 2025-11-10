################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
LP_MSPM0G3507/MKII.o: C:/Users/amosa/Intro_To_Embedded_Projects/LP_MSPM0G3507/MKII.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/Users/amosa/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/amosa/Intro_To_Embedded_Projects/PWM_PlayBuzzer_FSM" -I"C:/TI/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_07_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LP_MSPM0G3507/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LP_MSPM0G3507/bsp.o: C:/Users/amosa/Intro_To_Embedded_Projects/LP_MSPM0G3507/bsp.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/Users/amosa/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/amosa/Intro_To_Embedded_Projects/PWM_PlayBuzzer_FSM" -I"C:/TI/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_07_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LP_MSPM0G3507/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LP_MSPM0G3507/clock.o: C:/Users/amosa/Intro_To_Embedded_Projects/LP_MSPM0G3507/clock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/Users/amosa/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/amosa/Intro_To_Embedded_Projects/PWM_PlayBuzzer_FSM" -I"C:/TI/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_07_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LP_MSPM0G3507/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LP_MSPM0G3507/mspm0g350x_int.o: C:/Users/amosa/Intro_To_Embedded_Projects/LP_MSPM0G3507/mspm0g350x_int.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/Users/amosa/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/amosa/Intro_To_Embedded_Projects/PWM_PlayBuzzer_FSM" -I"C:/TI/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_07_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LP_MSPM0G3507/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LP_MSPM0G3507/timer.o: C:/Users/amosa/Intro_To_Embedded_Projects/LP_MSPM0G3507/timer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/Users/amosa/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/amosa/Intro_To_Embedded_Projects/PWM_PlayBuzzer_FSM" -I"C:/TI/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_07_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LP_MSPM0G3507/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LP_MSPM0G3507/uart.o: C:/Users/amosa/Intro_To_Embedded_Projects/LP_MSPM0G3507/uart.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/Users/amosa/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"C:/Users/amosa/Intro_To_Embedded_Projects/PWM_PlayBuzzer_FSM" -I"C:/TI/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_07_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LP_MSPM0G3507/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


