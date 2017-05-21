
if (NOT AVR_GCC)
    find_program(AVR_GCC avr-gcc)
endif ()

if (NOT AVR_G++)
    find_program(AVR_G++ avr-g++)
endif ()

if (NOT AVR_SIZE)
    find_program(AVR_SIZE avr-size)
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
if (NOT AVR_SIZE)
    message(FATAL_ERROR "Please install avr-size")
endif ()
if (NOT AVRDUDE)
    message(FATAL_ERROR "Please install avrdude")
endif ()

if (NOT AVR_MCU)
    message(FATAL_ERROR "Please specify AVR_MCU")
endif ()

if (NOT AVR_FCPU)
    message(FATAL_ERROR "Please specify AVR_FCPU")
endif ()


message(STATUS "Current avr-gcc is: ${AVR_GCC}")
message(STATUS "Current avr-g++ is: ${AVR_G++}")
message(STATUS "Current avr-objcopy is: ${AVR_OBJCOPY}")
message(STATUS "Current avr-size is: ${AVR_SIZE}")

message(STATUS "Current avrdude is: ${AVRDUDE}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current programmer port is: ${AVR_PROGRAMMER_PORT}")
message(STATUS "Current avrdude options are: ${AVRDUDE_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current FCPU is set to: ${AVR_FCPU}")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_C_COMPILER ${AVR_GCC})
set(CMAKE_CXX_COMPILER ${AVR_G++})
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

if (NOT AVR_PROGRAMMER)
    set(AVR_PROGRAMMER usbasp)
endif (NOT AVR_PROGRAMMER)


if (NOT AVR_PROGRAMMER_PORT)
    set(AVR_PROGRAMMER_PORT usb)
endif (NOT AVR_PROGRAMMER_PORT)


set(CMAKE_C_FLAGS "-Os -std=gnu99")
set(CMAKE_CXX_FLAGS "-Os -std=gnu++11")
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")


##########################################################################
# compiler options for all build types
##########################################################################
add_definitions("-DF_CPU=${AVR_FCPU}")
add_definitions("-fpack-struct")
add_definitions("-fshort-enums")
add_definitions("-Wall")
#add_definitions("-Werror")
#add_definitions("-pedantic")
#add_definitions("-pedantic-errors")
add_definitions("-funsigned-char")
add_definitions("-funsigned-bitfields")
add_definitions("-ffunction-sections")
add_definitions("-c")


function(add_avr_firmware FIRMWARE_NAME)
    set(elf_file ${FIRMWARE_NAME}.elf)
    set(hex_file ${FIRMWARE_NAME}.hex)
    set(map_file ${FIRMWARE_NAME}.map)
    set(eeprom_image ${FIRMWARE_NAME}-eeprom.hex)

    # elf file
    add_executable(${elf_file} EXCLUDE_FROM_ALL ${ARGN})

    set_target_properties(
            ${elf_file}
            PROPERTIES
            COMPILE_FLAGS "-mmcu=${AVR_MCU}"
            LINK_FLAGS "-mmcu=${AVR_MCU} -Wl,--gc-sections -mrelax -Wl,-Map,${map_file}"
    )

    add_custom_target(
            ${hex_file}
            COMMAND
            ${AVR_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}
            COMMAND
            ${AVR_SIZE} -B ${elf_file}
            DEPENDS ${elf_file}
    )

    # upload - with avrdude
    add_custom_target(
            ${FIRMWARE_NAME}_upload
            ${AVRDUDE} ${AVRDUDE_OPTIONS} -p ${AVR_MCU} -c ${AVR_PROGRAMMER}
            -U flash:w:${hex_file}
            -P ${AVR_PROGRAMMER_PORT}
            DEPENDS ${hex_file}
            COMMENT "Uploading ${hex_file} to ${AVR_MCU} using ${AVR_PROGRAMMER}"
    )

    # get status
    add_custom_target(
            ${FIRMWARE_NAME}_get_status
            ${AVRDUDE} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} -P ${AVR_PROGRAMMER_PORT} -n -v
            COMMENT "Get status from ${AVR_MCU}"
    )

endfunction(add_avr_firmware)
