# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

set(PICO_SDK_PATH ${CMAKE_CURRENT_LIST_DIR}/pico-sdk)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

#---FreeRTOS---
set(FREERTOS_KERNEL_PATH  ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS)

# Select the native compile PORT, see FreeRTOS/freertos_kernel-src/portable/CMakLists.cmake
set(FREERTOS_PORT "GCC_RP2040" CACHE STRING "" FORCE)
#--------------

project(VC5 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 1.0
add_executable(VC5 main.cpp)

pico_set_program_name(VC5 "VC5")
pico_set_program_version(VC5 "1.0")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(VC5 1)
pico_enable_stdio_usb(VC5 0)

# Add the standard library to the build
target_link_libraries(VC5
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(VC5 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)


#described here: https://github.com/raspberrypi/pico-sdk/issues/1659
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# add FreeRTOS to the main build
target_link_libraries(
    VC5 
    FreeRTOS-Kernel-Heap4
    )


pico_add_extra_outputs(VC5)
<<<<<<< Updated upstream

=======
>>>>>>> Stashed changes
