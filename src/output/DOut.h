#ifndef DOUT_H
#define DOUT_H

#include <stdint.h>

/**
 *  Wrapper around digital output.
 */
class DOut {

private:

    //  pointer to the output port
    volatile uint8_t *port;
    //  pointer to the output port's data direction register
    volatile uint8_t *ddr;
    //  pin the output is connected to
    uint8_t pin;

public:
    DOut(volatile uint8_t *PORT, volatile uint8_t *DDR, uint8_t PIN);

    ~DOut();

    void high(void);

    void low(void);

    void toggle(void);
};

#endif //DOUT_H
