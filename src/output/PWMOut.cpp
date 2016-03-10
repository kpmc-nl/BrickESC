#include <avr/io.h>
#include "PWMOut.h"

PWMOut::PWMOut(volatile uint8_t *ocr, uint8_t pin, uint8_t com00, uint8_t com01)
        : ocr(ocr), pin(pin), com00(com00), com01(com01) {
    init();
}

void PWMOut::init() {

    /* Set output pin direction */
    DDRB |= (1 << pin);

    /* Set WGM (Wave Form Generation Mode) to Fast PWM */
    TCCR0A |= (1 << WGM00) | (1 << WGM01);

    attach();
    
    /* Set CS (Clock Select) to internal clock 0; IE no prescaling */
    TCCR0B |= (1 << CS00);
}


void PWMOut::attach() {
    /* Set COM (Compare Output Mode) to Non Inverted PWM */
    TCCR0A &= ~(1 << com00);
    TCCR0A |= (1 << com01);

}

void PWMOut::detach() {
    /* Set COM (Compare Output Mode) to disabled */
    TCCR0A &= ~(1 << com00);
    TCCR0A &= ~(1 << com01);
}

void PWMOut::write(uint8_t value) {

    if (value == 0) {
        detach();
    } else {
        attach();
        *(ocr) = value;
    }


}