cmake_minimum_required(VERSION 3.15)
set(CMAKE_TOOLCHAIN_FILE "STM32F1_toolchain.cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(blink LANGUAGES C CXX ASM)
# Source files
add_executable(${PROJECT_NAME}
  ./src/main.c
  ./src/system_stm32f1xx.c
  ./include/startup_stm32f103xb.s
)
# Directories with headers
target_include_directories(${PROJECT_NAME} PRIVATE
  include
  /home/val/project/STM32_Projects/CMSIS/Core/Include)

# Linker options
target_link_options(${PROJECT_NAME} PRIVATE
  -Wl,--start-group
  -Wl,--end-group
  -mcpu=cortex-m0
  -mthumb
  -static)
# Define compile options
target_compile_options(${PROJECT_NAME} PRIVATE
  -mcpu=cortex-m0
  -mthumb
  -Os
  -ffunction-sections # Place functions in separate sections
  -fdata-sections # Place data in separate sections
  -fno-exceptions # Disable exceptions for bare-metal code
)
# Define defines here
target_compile_definitions(${PROJECT_NAME} PRIVATE
  STM32F103xB # Define your G0 series MCU
)
# Convert .elf firmware to .bin after build
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
)
# Print firmware Flash and RAM usage
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}
)