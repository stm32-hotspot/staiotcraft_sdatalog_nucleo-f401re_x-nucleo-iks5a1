cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
# STM32CubeMX generated symbols (macros)
set(MX_Defines_Syms 
	USE_HAL_DRIVER 
	STM32F401xE
    NO_PRINTF_USB
    NO_FLASH_MEM
#    NO_TIM3   # 1Sec MLC polling timer
#    NO_TIM5   # VDL precise timestamp timer
#   NO_UART_DMA
    DATA_READY_PULSED
    $<$<CONFIG:Debug>:DEBUG>
)

# STM32CubeMX generated include paths
set(MX_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../X-CUBE-MEMS1/Target
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Addons/PnPL/AppModel/Inc    
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Addons/PnPL/Components/Inc  
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/BSP/Components/iis2dulpx
    ${CMAKE_SOURCE_DIR}/Drivers/BSP/Components/ism6hg256x
    ${CMAKE_SOURCE_DIR}/Drivers/BSP/Components/Common
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/SimpleSerialTL/Inc
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/aspep_slave/Inc    
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/PnPLCompManager/Inc   
    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/parson     
    ${CMAKE_CURRENT_SOURCE_DIR}/../../VDATALOG/App 
)

# STM32CubeMX generated application sources
set(MX_Application_Src
    ${CMAKE_SOURCE_DIR}/X-CUBE-MEMS1/Target/custom_motion_sensors.c
    ${CMAKE_SOURCE_DIR}/X-CUBE-MEMS1/Target/custom_motion_sensors_ex.c	
    ${CMAKE_SOURCE_DIR}/X-CUBE-MEMS1/Target/MEMS_integration.c	
    ${CMAKE_SOURCE_DIR}/VDATALOG/App/vdatalog.c
    ${CMAKE_SOURCE_DIR}/VDATALOG/App/fsm.c
    ${CMAKE_SOURCE_DIR}/VDATALOG/App/timestamp.c
    ${CMAKE_SOURCE_DIR}/VDATALOG/App/Ism6hg256x_acc.c
    ${CMAKE_SOURCE_DIR}/VDATALOG/App/Ism6hg256x_gyro.c	
    ${CMAKE_SOURCE_DIR}/VDATALOG/App/Ism6hg256x_mlc_app.c	    
    ${CMAKE_SOURCE_DIR}/VDATALOG/App/app_vdatalog.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/AppModel/Src/App_model_Device_Information.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/AppModel/Src/App_model.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/AppModel/Src/App_model_Acquisition_Information.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/AppModel/Src/App_model_Log_Controller.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/AppModel/Src/App_model_Ism6hg256x_Acc.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/AppModel/Src/App_model_Ism6hg256x_Gyro.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/AppModel/Src/App_model_Ism6hg256x_Mlc.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/Device_Information_PnPL.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/PnPL_init.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/Ism6hg256x_Acc_PnPL.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/Ism6hg256x_Gyro_PnPL.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/Ism6hg256x_Mlc_PnPL.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/Acquisition_Information_PnPL.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/Device_Information_PnPL.c
    ${CMAKE_SOURCE_DIR}/Addons/PnPL/Components/Src/Log_Controller_PnPL.c    

    ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/parson/parson.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/PnPLCompManager/Src/IPnPLComponent.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/PnPLCompManager/Src/PnPLCompManager.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/aspep_slave/Src/aspep_slave.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/aspep_slave/Src/aspep_slave_config.c
#    ${CMAKE_SOURCE_DIR}/../../../../Middlewares/ST/aspep_slave/Src/usart_aspep_driver_no_dma.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/aspep_slave/Src/usart_aspep_driver_HAL.c
#    ${CMAKE_SOURCE_DIR}/../../../../Middlewares/ST/aspep_slave/Src/usart_aspep_driver_U5.c
    ${CMAKE_SOURCE_DIR}/Middlewares/ST/SimpleSerialTL/Src/simple_serial_tl.c

    ${CMAKE_CURRENT_SOURCE_DIR}/../../X-CUBE-MEMS1/Target/custom_motion_sensors.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../X-CUBE-MEMS1/Target/custom_motion_sensors_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/stm32f4xx_nucleo_bus.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/stm32f4xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/stm32f4xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../startup_stm32f401xe.s
)

# STM32 HAL/LL Drivers
set(STM32_Drivers_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Core/Src/system_stm32f4xx.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    ${CMAKE_SOURCE_DIR}/Drivers/BSP/Components/iis2dulpx/iis2dulpx_reg.c
    ${CMAKE_SOURCE_DIR}/Drivers/BSP/Components/iis2dulpx/iis2dulpx.c
    ${CMAKE_SOURCE_DIR}/Drivers/BSP/Components/ism6hg256x/ism6hg256x_reg.c
    ${CMAKE_SOURCE_DIR}/Drivers/BSP/Components/ism6hg256x/ism6hg256x.c
)

# Drivers Midllewares



# Link directories setup
set(MX_LINK_DIRS

)
# Project static libraries
set(MX_LINK_LIBS 
    STM32_Drivers
    ${TOOLCHAIN_LINK_LIBRARIES}
    
)
# Interface library for includes and symbols
add_library(stm32cubemx INTERFACE)
target_include_directories(stm32cubemx INTERFACE ${MX_Include_Dirs})
target_compile_definitions(stm32cubemx INTERFACE ${MX_Defines_Syms})

# Create STM32_Drivers static library
add_library(STM32_Drivers OBJECT)
target_sources(STM32_Drivers PRIVATE ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC stm32cubemx)


# Add STM32CubeMX generated application sources to the project
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MX_Application_Src})

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_DIRS})

# Add libraries to the project
target_link_libraries(${CMAKE_PROJECT_NAME} ${MX_LINK_LIBS})

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that STM32CubeMX code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()
