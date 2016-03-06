#ifndef PWMOUT_H
#define PWMOUT_H

#include <stdint.h>

/**
 * Wrapper around PWM output. Only Fast PWM, non inverted mode is supported ATM.
 */
class PWMOut {

private:
    // Timer Counter Control Register A
    volatile uint8_t *tccra;
    // Timer Counter Control Register B
    volatile uint8_t *tccrb;
    // Output Compare Register for the specified timer
    volatile uint8_t *ocr;
    // Data Direction Register for the specified pin
    volatile uint8_t *ddr;
    // Compare Output Mode 0 bit
    uint8_t com00;
    // Compare Output Mode 1 bit
    uint8_t com01;
    // Output pin
    uint8_t pin;

public:

    PWMOut(volatile uint8_t *tccra, volatile uint8_t *tccrb,
           volatile uint8_t *ocr, volatile uint8_t *ddr,
           uint8_t pin, uint8_t com00, uint8_t com01
    );

    void write(uint8_t value);

private:
    void init();

    void attach();

    void detach();
};


#endif // PWMOUT_H
