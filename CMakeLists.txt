cmake_minimum_required(VERSION 2.6)

# let cmake know we are cross-compiling
set(CMAKE_SYSTEM_NAME Generic)

project(speed-controller)

include(env.cmake)

set(AVR_MCU "attiny85")
set(AVR_FCPU "8000000")

add_definitions(-DARDUINO_ARCH_AVR)

include(cmake/arduino.cmake)
include(cmake/avr-toolchain.cmake)

include_directories(lib/ATTinyCore/avr/cores/tiny)
include_directories(lib/ATTinyCore/avr/variants/tinyX5)

include_directories(src)

set(ARDUINO_SOURCE_FILES
        lib/ATTinyCore/avr/variants/tinyX5/pins_arduino.h
        lib/ATTinyCore/avr/cores/tiny/WInterrupts.c
        lib/ATTinyCore/avr/cores/tiny/wiring.c
        lib/ATTinyCore/avr/cores/tiny/wiring_analog.c
        lib/ATTinyCore/avr/cores/tiny/wiring_digital.c
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
