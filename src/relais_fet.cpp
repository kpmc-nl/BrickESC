/**
 * Firmware for an RC speed controller with a circuit based on some FETs and some relais.
 */

#include <avr/io.h>
#include <output/DOut.h>
#include "output/PWMOut.h"
#include "input/RCInput.h"
#include <clock/Clock.h>


PWMOut fet(&TCCR0A, &TCCR0B, &OCR0A, &DDRB, PB0, COM0A0, COM0A1);
RCInput rcInput(&PORTB, &PINB, &DDRB, PB2, PCINT2);
DOut relais(&PORTB, &DDRB, PB1);

uint64_t pulse;

int main(void) {
    /* run at 8MHz */
    CLKPR = 0x80; // set system clock to 8mhz with no prescale
    CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order to set clock to 8 Mhz


    Clock::init();

    /* Proof of concept implementation! */
    while (1) {
        pulse = rcInput.getPulse();

        if (pulse > 1400 && pulse < 1600) {
            fet.write(0);
            continue;
        }

        if (pulse >= 1600) {
            relais.low();
            fet.write((pulse - 1600) * 255 / 400);
            continue;
        }

        if (pulse <= 1400) {
            relais.high();
            fet.write((1400 - pulse) * 255 / 400);
            continue;
        }
    }
}

