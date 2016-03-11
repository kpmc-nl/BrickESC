//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#ifndef HBRIDGE_DRIVER_RCINPUT_H
#define HBRIDGE_DRIVER_RCINPUT_H


#include <stdint.h>

/**
 * Defines a way to read a pulse from an RC receiver.
 * see: https://en.wikipedia.org/wiki/Servo_control
 */
class RCInput {

private:
    uint8_t pin;

    uint8_t prev_state;

    volatile uint64_t pulse_start;
    volatile uint64_t pulse_end;
    volatile uint32_t pulse_length;

    class RCInput *next;

    static RCInput *first;

public:
    RCInput(uint8_t pin);


    uint32_t getPulse();

    static void handleInterrupts();

private:
    void readInterrupt();

};


#endif //HBRIDGE_DRIVER_RCINPUT_H
