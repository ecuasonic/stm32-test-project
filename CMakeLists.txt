cmake_minimum_required(VERSION 3.15)
project(stm32_research C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TARGET_NAME firmware.elf)
set(TEST_NAME "stm1" CACHE STRING "testname for compilation")

# ------------------------
#         Toolchain
# ------------------------
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_OBJCOPY /usr/bin/arm-none-eabi-objcopy)

# ------------------------
#         Directories
# ------------------------
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

file(GLOB SRC_FILES CONFIGURE_DEPENDS 
    "${SRC_DIR}/${TEST_NAME}/*.c" 
    "${SRC_DIR}/oled_images/*.c"
    "${SRC_DIR}/periph/*.c"
    "${SRC_DIR}/core_stm/*.c"
    "${SRC_DIR}/*.c" 
)

# ------------------------
#      Compiler flags
# ------------------------
set (CFLAGS -mcpu=cortex-m3 -mthumb -Os -ffreestanding)

# ------------------------
#      Build ELF
# ------------------------
add_executable(${TARGET_NAME} ${SRC_FILES})

set(LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/linker.ld")
set_target_properties(${TARGET_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY    ${BUILD_DIR}
    LINK_FLAGS                  "-T${LINKER_SCRIPT} -nostdlib"
)
target_include_directories(${TARGET_NAME} PRIVATE 
    ${INC_DIRS}
)
target_compile_options(${TARGET_NAME} PRIVATE
    ${CFLAGS}
)

# ------------------------
#  Generate BIN from ELF
# ------------------------
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware.elf> ${BUILD_DIR}/firmware.bin
    COMMENT "Creating binary firmware"
)

# ------------------------
#     Flash Target
# ------------------------
add_custom_target(flash
    COMMAND st-flash --reset write ${BUILD_DIR}/firmware.bin 0x08000000
    DEPENDS ${BUILD_DIR}/firmware.bin
    COMMENT "Flashing firmware"
)

# ------------------------
#     Flash Target
# ------------------------
set_directory_properties(
    PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${BUILD_DIR}/firmware.bin"
)