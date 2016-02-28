#include <avr/io.h>
#include "clock/Clock.h"
#include "output/PWMOut.h"
#include "input/RCInput.h"


PWMOut pwm0(&TCCR0A, &TCCR0B, &OCR0A, &DDRB, PB0, COM0A0, COM0A1);
PWMOut pwm1(&TCCR0A, &TCCR0B, &OCR0B, &DDRB, PB1, COM0B0, COM0B1);

RCInput rcInput(&PORTB, &PINB, &DDRB, PB3, PCINT3);

uint64_t pulse;

int main(void) {
    /* run at 8MHz */
    CLKPR = 0x80; // set system clock to 8mhz with no prescale
    CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order to set clock to 8 Mhz

    Clock::init();


    while (1) {
        pulse = rcInput.getPulse();

        if (pulse > 1450 && pulse < 1550) {
            pwm0.write(0);
            pwm1.write(0);
            continue;
        }

        if (pulse >= 1550) {
            pwm0.write(0);
            pwm1.write((pulse - 1550) * 255 / 450);
            continue;
        }

        if (pulse <= 1450) {
            pwm1.write(0);
            pwm0.write((1450 - pulse) * 255 / 450);
            continue;
        }
    }
}

