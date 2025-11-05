################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
LP_MSPM0G3507/%.o: ../LP_MSPM0G3507/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/Users/amosa/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/amosa/workspace_ccstheia/Booster_Projects/MKII_ToggleMuticolorLED" -I"C:/TI/mspm0_sdk_2_07_00_05/source/third_party/CMSIS/Core/Include" -I"C:/TI/mspm0_sdk_2_07_00_05/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LP_MSPM0G3507/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


