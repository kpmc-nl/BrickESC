project(hbridge-driver)

cmake_minimum_required(VERSION 2.6)

# Change this to the directory of your Arduino installation.
# On a Mac, it will be something like '/Applications/Arduino.app/Contents/Java'
set(ARDUINO_ROOT "/Applications/Arduino.app/Contents/Java")

set(AVRDUDE_PROGRAMMER_FLAGS -cstk500v1 -P/dev/cu.usbmodem1421 -b19200)
set(AVR_MCU "attiny45")
set(AVR_FCPU "1000000")

include(cmake/avr-arduino.cmake)

set(SOURCE_FILES
        # Add more source files if you need to.
        src/main.cpp
        src/Led.cpp src/Led.h)

add_executable(firmware ${SOURCE_FILES})