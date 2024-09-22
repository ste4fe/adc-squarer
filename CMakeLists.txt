set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(CPPCHECK cppcheck)
# set(CMAKE_C_CPPCHECK cppcheck)
message("${CMAKE_TOOLCHAIN_PATH}")

get_filename_component(ProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "-" ProjectName ${ProjectName})

project(${ProjectName} LANGUAGES C CXX ASM)
set(CMAKE_PROJECT_VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 17)

# set(CMAKE_BUILD_TYPE Release)

add_compile_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# Оптимизации
add_compile_options(-funroll-loops -fomit-frame-pointer -finline-functions)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)
add_compile_options(-O0 -g)
# add_compile_options()

include_directories(src Drivers/APM32F10x_StdPeriphDriver/inc Drivers/CMSIS/Include Drivers/Geehy/APM32F10x/Include)

add_definitions(-DAPM32F10X_MD)

# PROJECT DEFINITIONS
# Firmware version
add_definitions(-DFIRMWARE_VERSION="${CMAKE_PROJECT_VERSION}")

# Unique ID Address
add_definitions(-DMCUID_ADDRESS=0x1ffff7e8)

file(GLOB_RECURSE SOURCES "src/*.*" "Drivers/*.*" "Lib/*.*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker-script.ld)

add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m3 -mthumb -mthumb-interwork)
add_link_options(-specs=nosys.specs)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

# add_custom_target(static-analisys COMMAND ${CPPCHECK} --rule=^MISRA --enable=all ${CMAKE_SOURCE_DIR}/src --suppress=missingIncludeSystem .)
# add_dependencies(${PROJECT_NAME}.elf static-analisys)
