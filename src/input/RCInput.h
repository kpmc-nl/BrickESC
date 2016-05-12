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
    const static uint8_t SAMPLE_SIZE = 12;

    uint8_t pin;

    uint8_t prev_state;

    uint64_t last_pulse_start;
    uint64_t last_pulse_end;
    uint32_t pulse_length_sample[SAMPLE_SIZE];
    uint8_t sample_counter;

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
