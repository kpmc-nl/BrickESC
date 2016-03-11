//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#include <avr/interrupt.h>
#include "Clock.h"

#ifdef ENABLE_CLOCK

static volatile uint64_t overflow_count = 0;

ISR(TIM1_OVF_vect) {
    overflow_count++;
}

void Clock::init() {




    /* configure timer so it will update the counter every 1us.
     * 1MHz and 8MHZ clock frequencies are ideal for this :)
     * Timer overflow will now happen every 256us. */
#if F_CPU == 1000000L
    TCCR1 |= (1 << CS10); // no prescaling
#elif F_CPU == 8000000L
    TCCR1 |= (1 << CS12); // 1/8th prescaling
#else
#error "Unsupported CPU frequency"
#endif


    // enable timer overflow interrupt for timer 1
    TIMSK |= 1 << TOIE1;

    TCNT1 = 0;
}

uint64_t Clock::millis() {
    /* TODO: there might be a better way to do this. For now it will do. */
    return micros() / 1000;
}

uint64_t Clock::micros() {
    uint64_t count;
    cli();
    count = overflow_count;
    sei();
    /* Bit shift 8 to the left is the same as multiplication by 256, but I suppose this is faster. */
    return (count * 256) + TCNT1;
}

#endif