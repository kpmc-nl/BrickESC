#include <avr/io.h>
#include <util/delay.h>
#include "output/PWMOut.h"


PWMOut pwm0(&TCCR0A, &TCCR0B, &OCR0A, &DDRB, PB0, COM0A0, COM0A1);
PWMOut pwm1(&TCCR0A, &TCCR0B, &OCR0B, &DDRB, PB1, COM0B0, COM0B1);

int main(void) {


    uint8_t x;
    while (1) {

        for (x = 0; x < 255; x++) {
            pwm0.write(x);
            pwm1.write(255 - x);
            _delay_ms(4);
        }

        for (x = 255; x > 0; x--) {
            pwm0.write(x);
            pwm1.write(255 - x);
            _delay_ms(4);
        }

    }
}
