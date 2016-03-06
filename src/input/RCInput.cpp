//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#include <avr/interrupt.h>
#include "RCInput.h"
#include "clock/Clock.h"

#define NEUTRAL_PULSE 1500
#define MAX_PULSE 2500
#define MIN_PULSE 500

RCInput *RCInput::first;

RCInput::RCInput(volatile uint8_t *port, volatile uint8_t *pin_reg, volatile uint8_t *ddr,
                 uint8_t read_pin, uint8_t int_pin)
        : port(port), pin_reg(pin_reg), ddr(ddr),
          read_pin(read_pin), int_pin(int_pin),
          next(0),
          pulse_start(0), pulse_length(0) {
    next = first;
    first = this;
    pulse_length = NEUTRAL_PULSE;
    init();
}

void RCInput::init() {

    cli();
    GIMSK |= (1 << PCIE); // turns on pin change interrupts
    PCMSK |= (1 << int_pin); // turn on interrupts on specified pin.
    sei();

    *(ddr) &= ~(1 << read_pin); // read pin as input
}

uint64_t RCInput::getPulse() {
    if (Clock::micros() - pulse_start > 100000) {
        // if we didn't get an update for 100ms, suspect receiver disconnect.
        return NEUTRAL_PULSE;
    }
    if (pulse_length < MIN_PULSE) {
        return MIN_PULSE;
    } else if (pulse_length > MAX_PULSE) {
        return MAX_PULSE;
    } else {
        return pulse_length;
    }
}

void RCInput::readInterrupt() {

    uint8_t state = *(pin_reg) & (1 << read_pin);

    if (state != prev_state) {

        if (state) {
            // pin is high
            pulse_start = Clock::micros();
        } else {
            // pin is low
            pulse_length = Clock::micros() - pulse_start;
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