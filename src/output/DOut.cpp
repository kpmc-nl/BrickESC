#include <avr/io.h>
#include "DOut.h"

DOut::DOut(uint8_t pin)
        : pin(pin) {
    /* Set output pin direction */
    DDRB |= (1 << pin);
}

void DOut::high(void) {
    PORTB |= (1 << pin);
}

void DOut::low(void) {
    PORTB &= ~(1 << pin);
}

void DOut::toggle(void) {
    PORTB ^= (1 << pin);
}