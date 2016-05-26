cmake_minimum_required(VERSION 2.6)

project(speed-controller)

set(AVRDUDE_PROGRAMMER_FLAGS -cusbasp)
set(AVR_MCU "attiny841")
set(AVR_FCPU "8000000")

add_definitions(-DARDUINO_ARCH_AVR)

add_definitions(-DLOOPTIME=1500) #microseconds
add_definitions(-DRC_SAMPLE_SIZE=11)

file(STRINGS "attiny-core-path.txt" ATTINY_CORE_PATH)
set(AVRDUDE_CONFIG "${ATTINY_CORE_PATH}/avr/avrdude.conf")

include(cmake/arduino-1.6.cmake)
include(cmake/avr-toolchain.cmake)


include_directories(${ATTINY_CORE_PATH}/avr/cores/tinymodern)
include_directories(src)

set(ARDUINO_SOURCE_FILES

        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/pins_arduino.c
        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/WInterrupts.c
        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/wiring.c
        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/wiring_analog.c
        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/wiring_digital.c
        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/wiring_pulse.c
        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/wiring_shift.c
        ${ATTINY_CORE_PATH}/avr/cores/tinymodern/WMath.cpp

        )

set(SOURCE_FILES
        ${ARDUINO_SOURCE_FILES}

        src/main.cpp
        src/Controller.cpp src/Controller.h
        src/RCInput.cpp src/RCInput.h
        src/Settings.cpp src/Settings.h
        src/util.cpp src/util.h
        )


add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})