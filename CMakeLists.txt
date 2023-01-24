#-- Service --------------------------------------------------------------------
SET(CMAKE_TOOLCHAIN_FILE ./gcc_cm4f.cmake)
ENABLE_LANGUAGE(ASM)
CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0)
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
ENDIF()
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-- Project config -------------------------------------------------------------
PROJECT(STM32F411)                    # Project name
SET(MCUNAME         STM32F411)     # MCU name

#-- Defines --------------------------------------------------------------------
ADD_DEFINITIONS(-D${MCUNAME})
ADD_DEFINITIONS(-DOSECLK_VAL=${OSECLK_VAL})
ADD_DEFINITIONS(-DSYSCLK_${SYSCLK})
ADD_DEFINITIONS(-DCKO_${CKO})

#-- Project paths --------------------------------------------------------------
SET(APP_PATH                app)
SET(PLATFORM_PATH           STM32F411_sdk/platform)
SET(DEVICE_SRC_PATH         ${PLATFORM_PATH}/Device/ST/${MCUNAME}/Source)
SET(DEVICE_INC_PATH         ${PLATFORM_PATH}/Device/ST/${MCUNAME}/Include)
SET(CMSIS_CORE_INC_PATH     ${PLATFORM_PATH}/CMSIS/Core/Include)


#-- Include dirs ---------------------------------------------------------------
INCLUDE_DIRECTORIES(${APP_PATH})
INCLUDE_DIRECTORIES(${CMSIS_CORE_INC_PATH})
INCLUDE_DIRECTORIES(${DEVICE_INC_PATH})
INCLUDE_DIRECTORIES(${BSP_INC_PATH})

#-- Sources list ---------------------------------------------------------------
FILE(GLOB_RECURSE APP_SRC ${APP_PATH}/*.c  ${DEVICE_SRC_PATH}/*.c)
LIST(APPEND APP_SRC ${DEVICE_SRC_PATH}/GCC/startup_${MCUNAME}.s)


#-- Linker script --------------------------------------------------------------
SET(LDSCRIPT ${CMAKE_SOURCE_DIR}/${DEVICE_SRC_PATH}/GCC/${MCUNAME}RETx_FLASH.ld)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LDSCRIPT} -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map -Wl,--print-memory-usage")

#-- Project linking ------------------------------------------------------------
ADD_EXECUTABLE(${PROJECT_NAME}.elf ${APP_SRC})
TARGET_LINK_LIBRARIES(${PROJECT_NAME}.elf)

#-- Custom commands ------------------------------------------------------------
ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} "-Oihex" ${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex
        COMMAND ${CMAKE_OBJCOPY} "-Obinary" ${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
        COMMAND ${CMAKE_OBJDUMP} "-DS" ${PROJECT_NAME}.elf > ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dasm
        COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf)
