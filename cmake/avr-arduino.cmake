# AVR toolchain from Arduino for regular AVR programming.

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER ${ARDUINO_ROOT}/hardware/tools/avr/bin/avr-gcc)
set(CMAKE_ASM_COMPILER ${ARDUINO_ROOT}/hardware/tools/avr/bin/avr-gcc)
set(CMAKE_CXX_COMPILER ${ARDUINO_ROOT}/hardware/tools/avr/bin/avr-g++)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
enable_language(ASM)

# C only fine tunning
set(TUNNING_FLAGS "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")

set(CMAKE_CXX_FLAGS "-mmcu=${AVR_MCU} -DF_CPU=${AVR_FCPU} -Os")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${TUNNING_FLAGS} -Wall -Wstrict-prototypes -std=gnu99")
set(CMAKE_ASM_FLAGS "-mmcu=${AVR_MCU}")

set(AVRDUDE_CONFIG "${ARDUINO_ROOT}/hardware/tools/avr/etc/avrdude.conf")


find_program(AVROBJCOPY ${ARDUINO_ROOT}/hardware/tools/avr/bin/avr-objcopy)
find_program(AVRDUDE ${ARDUINO_ROOT}/hardware/tools/avr/bin/avrdude)

# FIXME: Forcing target name to be "firmware"
if (AVROBJCOPY AND AVRDUDE)
    add_custom_target(hex)
    add_dependencies(hex firmware)

    add_custom_command(TARGET hex POST_BUILD
            COMMAND ${AVROBJCOPY} -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/firmware firmware.hex
            )

    add_custom_target(flash)
    add_dependencies(flash hex)

    add_custom_command(TARGET flash POST_BUILD
            COMMAND ${AVRDUDE} -C${AVRDUDE_CONFIG} -v -p${AVR_MCU} ${AVRDUDE_PROGRAMMER_FLAGS} -Uflash:w:firmware.hex:i
            )
endif ()