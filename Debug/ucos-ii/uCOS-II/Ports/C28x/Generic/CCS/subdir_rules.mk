################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_a.obj: ../ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_a.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/common/include" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/config" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU/C28x/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-LIB" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Source" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_a.d_raw" --obj_directory="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_c.obj: ../ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/common/include" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/config" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU/C28x/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-LIB" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Source" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_c.d_raw" --obj_directory="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_i.obj: ../ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_i.asm $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/common/include" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/config" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU/C28x/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-LIB" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Source" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_cpu_i.d_raw" --obj_directory="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_dbg.obj: ../ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_dbg.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/headers/include" --include_path="C:/ti/c2000/C2000Ware_3_03_00_00/device_support/f2833x/common/include" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/config" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-CPU/C28x/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uC-LIB" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" --include_path="D:/ccsv7_workspace/ucOSport_DSP28335_Demo/ucos-ii/uCOS-II/Source" --include_path="C:/ti/ccsv7/tools/compiler/ti-cgt-c2000_16.9.6.LTS/include" -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS/os_dbg.d_raw" --obj_directory="ucos-ii/uCOS-II/Ports/C28x/Generic/CCS" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


