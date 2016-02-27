#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "output/PWMOut.h"
#include "output/DOut.h"


PWMOut pwm0(&TCCR0A, &TCCR0B, &OCR0A, &DDRB, PB0, COM0A0, COM0A1);
DOut led1(&PORTB, &DDRB, PB1);
//PWMOut pwm1(&TCCR0A, &TCCR0B, &OCR0B, &DDRB, PB1, COM0B0, COM0B1);

int main(void) {

    cli();
    GIMSK |= (1 << PCIE); // turns on pin change interrupts
    PCMSK |= (1 << PCINT3); // turn on interrupts on pins PB2,
    sei();

    DDRB &= ~(1 << PB3); // PB3 as input
    PORTB |= (1 << PB3); // internal pullup pb3



    uint8_t x;
    while (1) {
        for (x = 0; x < 255; x++) {
            pwm0.write(x);
            _delay_ms(4);
        }
        _delay_ms(1000);
    }
}

ISR(PCINT0_vect) {

    if (!(PINB & (1 << PB3))) {
        led1.toggle();
    }

}