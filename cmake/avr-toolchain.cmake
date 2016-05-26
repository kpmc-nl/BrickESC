if (NOT AVR_GCC)
    find_program(AVR_GCC avr-gcc)
endif ()
if (NOT AVR_G++)
    find_program(AVR_G++ avr-g++)
endif ()
if (NOT AVR_OBJCOPY)
    find_program(AVR_OBJCOPY avr-objcopy)
endif ()
if (NOT AVRDUDE)
    find_program(AVRDUDE avrdude)
endif ()

if (NOT AVR_GCC)
    message(FATAL_ERROR "Please install avr-gcc")
endif ()
if (NOT AVR_G++)
    message(FATAL_ERROR "Please install avr-g++")
endif ()
if (NOT AVR_OBJCOPY)
    message(FATAL_ERROR "Please install avr-objcopy")
endif ()
if (NOT AVRDUDE)
    message(FATAL_ERROR "Please install avrdude")
endif ()

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER ${AVR_GCC})
set(CMAKE_CXX_COMPILER ${AVR_G++})
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

# C only fine tuning
set(C_TUNING_FLAGS "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")

set(CMAKE_CXX_FLAGS "-mmcu=${AVR_MCU} -DF_CPU=${AVR_FCPU} -Os")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} ${C_TUNING_FLAGS} -Wall -Wstrict-prototypes -std=gnu99")

add_custom_target(hex)
add_dependencies(hex ${CMAKE_PROJECT_NAME})

add_custom_command(TARGET hex POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}.hex
        )

add_custom_target(flash)
add_dependencies(flash hex)

add_custom_command(TARGET flash POST_BUILD
        COMMAND ${AVRDUDE} -v -C${AVRDUDE_CONFIG} -p${AVR_MCU} ${AVRDUDE_PROGRAMMER_FLAGS} -Uflash:w:${CMAKE_PROJECT_NAME}.hex:i
        )