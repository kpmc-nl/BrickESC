cmake_minimum_required(VERSION 2.6)

# let cmake know we are cross-compiling
set(CMAKE_SYSTEM_NAME Generic)

project(speed-controller C)

include(env.cmake)

set(AVR_MCU "attiny841")
set(AVR_FCPU "8000000")

add_definitions(-DARDUINO_ARCH_AVR)

set(AVRDUDE_OPTIONS -C${CMAKE_CURRENT_SOURCE_DIR}/avrdude.conf)

include(cmake/arduino.cmake)
include(cmake/avr-toolchain.cmake)


include_directories(lib/ATTinyCore/avr/cores/tinymodern)
include_directories(src)

set(ARDUINO_SOURCE_FILES
        lib/ATTinyCore/avr/cores/tinymodern/pins_arduino.c
        lib/ATTinyCore/avr/cores/tinymodern/WInterrupts.c
        lib/ATTinyCore/avr/cores/tinymodern/wiring.c
        lib/ATTinyCore/avr/cores/tinymodern/wiring_analog.c
        lib/ATTinyCore/avr/cores/tinymodern/wiring_digital.c
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

add_avr_firmware(${PROJECT_NAME} ${SOURCE_FILES})