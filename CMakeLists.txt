cmake_minimum_required(VERSION 2.6)

# let cmake know we are cross-compiling
set(CMAKE_SYSTEM_NAME Generic)

project(speed-controller)

include(cmake/build-env.cmake)

set(AVR_MCU "attiny841")
set(AVR_FCPU "8000000")

add_definitions(-DARDUINO_ARCH_AVR)

add_definitions(-DCMAKE_BUILD)

set(AVRDUDE_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/avrdude.conf")

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
        )

set(SOURCE_FILES
        ${ARDUINO_SOURCE_FILES}

        src/rc.h
        src/pinout.h

        src/motor.h src/motor.c
        src/rc_input.c src/rc_input.h
        src/controller.h src/controller.c
        src/util.c src/util.h

        src/main.c
        src/settings.c src/settings.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})