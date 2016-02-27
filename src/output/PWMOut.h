#ifndef PWMOUT_H
#define PWMOUT_H

#include <stdint.h>

/**
 * \brief Wrapper around PWM output. Only Fast PWM, non inverted mode is supported ATM.
 */
class PWMOut {

private:

    volatile uint8_t *tccra;
    volatile uint8_t *tccrb;
    volatile uint8_t *ocr;
    volatile uint8_t *ddr;
    uint8_t com00;
    uint8_t com01;
    uint8_t pin;


public:

    /**
     * \brief Constructor for the PWMOut class.
     * \param tccra Timer Counter Control Register A
     * \param tccrb Timer Counter Control Register B
     * \param ocr   Output Compare Register for the specified timer
     * \param ddr   Data Direction Register for the specified pin
     * \param pin   Output pin
     * \param com00 Compare Output Mode 0 bit
     * \param com01 Compare Output Mode 1 bit
     */
    PWMOut(volatile uint8_t *tccra, volatile uint8_t *tccrb,
           volatile uint8_t *ocr, volatile uint8_t *ddr,
           uint8_t pin, uint8_t com00, uint8_t com01
    );

    /*! Destructor */
    ~PWMOut();

    /*! Write a value (0-255) to the PWM Output. */
    void write(uint8_t value);

private:
    void init();
};


#endif // PWMOUT_H
