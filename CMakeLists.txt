project(speed-controller)

cmake_minimum_required(VERSION 2.6)

set(AVRDUDE_PROGRAMMER_FLAGS -cstk500v1 -P/dev/cu.usbmodem1421 -b19200)
set(AVR_MCU "attiny45")
set(AVR_FCPU "8000000")

include(cmake/avr-toolchain.cmake)

add_definitions(-DENABLE_CLOCK)

include_directories(src)

set(SOURCE_FILES
        # To switch between firmware types, enable one of the following
        src/relay_fet.cpp
        # src/dual_hbridge.cpp

        # shared sources
        src/output/DOut.cpp src/output/DOut.h
        src/output/PWMOut.cpp src/output/PWMOut.h
        src/clock/Clock.cpp src/clock/Clock.h
        src/input/RCInput.cpp src/input/RCInput.h)

add_executable(${CMAKE_PROJECT_NAME}

        ${SOURCE_FILES}
        )
