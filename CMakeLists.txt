# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(datalogger_main C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories( ${CMAKE_SOURCE_DIR}/lib ) # Inclui os files .h na pasta lib
add_subdirectory(lib/FatFs_SPI)    

# Add executable. Default name is the project name, version 0.1

add_executable(${PROJECT_NAME}  
        datalogger_main.c   # Arquivo principal do projeto
        lib/imu_driver.c        # Módulo do IMU
        lib/sd_manager.c        # Módulo do Cartão SD
        lib/ui_manager.c        # Módulo de UI
        lib/buzzer.c            # Módulo do Buzzer
        lib/ssd1306.c
        hw_config.c             
        )

pico_set_program_name(datalogger_main "datalogger_main")
pico_set_program_version(datalogger_main "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(datalogger_main 1)
pico_enable_stdio_usb(datalogger_main 1)

# Add the standard library to the build
target_link_libraries(datalogger_main
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(datalogger_main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(${PROJECT_NAME} 
        pico_stdlib 
        hardware_i2c
        FatFs_SPI
        hardware_spi
        hardware_pwm
        )

pico_add_extra_outputs(datalogger_main)

