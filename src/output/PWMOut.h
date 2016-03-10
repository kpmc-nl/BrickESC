#ifndef PWMOUT_H
#define PWMOUT_H

#include <stdint.h>

/**
 * Wrapper around PWM output. Only Fast PWM, non inverted mode is supported ATM.
 *
 * Currently, the implementation is tied directly to timer0.
 */
class PWMOut {

private:
    // Output Compare Register
    volatile uint8_t *ocr;
    // Compare Output Mode 0 bit
    uint8_t com00;
    // Compare Output Mode 1 bit
    uint8_t com01;

    // Output pin
    uint8_t pin;

public:

    PWMOut(volatile uint8_t *ocr, uint8_t pin, uint8_t com00, uint8_t com01);

    void write(uint8_t value);

private:
    void init();

    void attach();

    void detach();
};


#endif // PWMOUT_H
