//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#include <avr/interrupt.h>
#include "Clock.h"

#ifdef ENABLE_CLOCK
static volatile uint64_t _millis = 0;
static volatile uint16_t _1000us = 0;

ISR(TIM1_OVF_vect) {
#if F_CPU >= 16000000L
    // At 16MHz, timer overflow happens every 0.016ms
        _1000us += 16;
#elif F_CPU >= 8000000L
    // At 8MHz, timer overflow happens every 0.032ms
    _1000us += 32;
#elif F_CPU >= 1000000L
    // At 1MHz, timer overflow happens every 0.256ms
_1000us += 32;
#endif
    while (_1000us > 1000) {
        _millis++;
        _1000us -= 1000;
    }
}

void Clock::init() {
    /* interrup setup */
    TCCR1 |= (1 << CS00); // no prescaling; ie timer overflow interrupt every 32us
    // enable timer overflow interrupt for timer 1
    TIMSK |= 1 << TOIE1;
}

uint64_t Clock::millis() {
    uint64_t m;
    cli();
    m = _millis;
    sei();
    return m;
}

uint64_t Clock::micros() {
    uint64_t m;
    cli();
    m = 1000 * _millis;
    m += _1000us;
    sei();
    return m;
}

#endif