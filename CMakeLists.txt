project(hbridge-driver)

cmake_minimum_required(VERSION 2.6)


set(AVRDUDE_PROGRAMMER_FLAGS -cstk500v1 -P/dev/cu.usbmodem1421 -b19200)
set(AVR_MCU "attiny45")
set(AVR_FCPU "1000000")

include(cmake/avr-toolchain.cmake)


set(SOURCE_FILES
        src/main.cpp
        src/output/DOut.cpp src/output/DOut.h
        src/output/PWMOut.cpp src/output/PWMOut.h

        )

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})