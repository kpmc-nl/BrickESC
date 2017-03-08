
if (NOT ARDUINO_PATH)
    message(FATAL_ERROR "Specify the Arduino SDK path in the textfile arduino-path.txt")
endif ()

enable_language(ASM)

set(ARDUINO_CORE_DIR "${ARDUINO_PATH}/hardware/arduino/avr/cores/arduino/arduino.cmake")
set(ARDUINO_PINS_DIR "${ARDUINO_PATH}/hardware/arduino/avr/variants/${ARDUINO_VARIANT}")

if (NOT AVRDUDE_CONFIG)
    set(AVRDUDE_CONFIG "${ARDUINO_PATH}/hardware/tools/avr/etc/avrdude.conf")
endif ()

if (NOT AVR_GCC)
    set(AVR_GCC ${ARDUINO_PATH}/hardware/tools/avr/bin/avr-gcc)
endif()

if (NOT AVR_G++)
    set(AVR_G++ ${ARDUINO_PATH}/hardware/tools/avr/bin/avr-g++)
endif()

if (NOT AVR_OBJCOPY)
    set(AVR_OBJCOPY ${ARDUINO_PATH}/hardware/tools/avr/bin/avr-objcopy)
endif()

if (NOT AVRDUDE)
    set(AVRDUDE ${ARDUINO_PATH}/hardware/tools/avr/bin/avrdude -C${AVRDUDE_CONFIG})
endif()