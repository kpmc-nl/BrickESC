#include <avr/io.h>
#include "PWMOut.h"

PWMOut::PWMOut(volatile uint8_t *tccra, volatile uint8_t *tccrb,
               volatile uint8_t *ocr, volatile uint8_t *ddr,
               uint8_t pin, uint8_t com00, uint8_t com01)
        : tccra(tccra), tccrb(tccrb), ocr(ocr), ddr(ddr), pin(pin), com00(com00), com01(com01) {
    init();
}

PWMOut::~PWMOut() {

}

void PWMOut::init() {

    /* Set output pin direction */
    *(ddr) |= (1 << pin);

    /* Set WGM (Wave Form Generation Mode) to Fast PWM */
    *(tccra) |= (1 << WGM00) | (1 << WGM01);

    attach();

    *(tccra) &= ~(1 << com00);
    *(tccra) |= (1 << com01);

    /* Set CS (Clock Select) to internal clock 0; IE no prescaling */
    *(tccrb) |= (1 << CS00);
}


void PWMOut::attach() {
    /* Set COM (Compare Output Mode) to Non Inverted PWM */
    *(tccra) &= ~(1 << com00);
    *(tccra) |= (1 << com01);

}

void PWMOut::detach() {
    *(tccra) &= ~(1 << com00);
    *(tccra) &= ~(1 << com01);
}

void PWMOut::write(uint8_t value) {

    if (value == 0) {
        detach();
    } else {
        attach();
        *(ocr) = value;
    }


}