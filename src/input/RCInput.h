//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#ifndef HBRIDGE_DRIVER_RCINPUT_H
#define HBRIDGE_DRIVER_RCINPUT_H


#include <stdint.h>


class RCInput {

private:
    volatile uint8_t *port;
    volatile uint8_t *ddr;
    volatile uint8_t *pin_reg;
    uint8_t read_pin;
    uint8_t int_pin;

    uint8_t prev_state;

    volatile uint64_t pulse_start;
    volatile uint64_t pulse_length;

    class RCInput *next;
    static RCInput *first;

public:
    RCInput(volatile uint8_t *port, volatile uint8_t *pin_reg, volatile uint8_t *ddr, uint8_t read_pin,
            uint8_t int_pin);


    uint64_t getPulse();

    static void handleInterrupts();

private:
    void readInterrupt();
    void init();

};


#endif //HBRIDGE_DRIVER_RCINPUT_H
