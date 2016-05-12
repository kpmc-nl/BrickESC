//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#include <avr/interrupt.h>
#include "RCInput.h"
#include "clock/Clock.h"
#include "util/Util.h"

#define NEUTRAL_PULSE 1500
#define MAX_PULSE 2500
#define MIN_PULSE 500

RCInput *RCInput::first;

RCInput::RCInput(uint8_t pin)
        : pin(pin),
          next(0),
          last_pulse_start(0), last_pulse_end(0) {
    next = first;
    first = this;

    GIMSK |= (1 << PCIE); // turns on pin change interrupts
    PCMSK |= (1 << pin); // turn on interrupts on specified pin.
    DDRB &= ~(1 << pin); // read pin as input


    for (uint8_t i = 0; i < SAMPLE_SIZE; i++) {
        pulse_length_sample[i] = NEUTRAL_PULSE;
    }

}


uint32_t RCInput::getPulse() {
    if (Clock::micros() - last_pulse_start > 100000) {
        // if we didn't get an update for 100ms, suspect receiver disconnect.
        return NEUTRAL_PULSE;
    }

    uint32_t result = Util::median(pulse_length_sample, SAMPLE_SIZE);

    if (result < MIN_PULSE) {
        return MIN_PULSE;
    } else if (result > MAX_PULSE) {
        return MAX_PULSE;
    } else {
        return result;
    }
}

void RCInput::readInterrupt() {

    uint8_t state = PINB & (1 << pin);
    uint64_t now = Clock::micros();

    if (state != prev_state) {

        if (state) {
            // pin is high
            uint64_t low_time = now - last_pulse_end;
            /* Verify that the input is low for a period between 10 and 40 ms.
             * If not, it is not a valid RC PWM signal. If the signal is not valid,
             * the getPulse method will eventually notice, because the pulse_start
             * value will not be updated anymore. */
            if (low_time > 10000 && low_time < 40000) {
                if (sample_counter == SAMPLE_SIZE) {
                    sample_counter = 0;
                }
                pulse_length_sample[sample_counter] = last_pulse_end - last_pulse_start;
                last_pulse_start = now;
                sample_counter++;
            }
        } else {
            // pin is low
            last_pulse_end = now;
        }
        prev_state = state;
    }


}

void RCInput::handleInterrupts() {
    RCInput *i = first;

    while (i != 0) {
        i->readInterrupt();
        i = i->next;
    }
}

ISR(PCINT0_vect) {
    RCInput::handleInterrupts();
}