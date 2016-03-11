#ifndef DOUT_H
#define DOUT_H

#include <stdint.h>

/**
 *  Wrapper around digital output. (HIGH/LOW)
 */
class DOut {

private:
    //  pin the output is connected to
    uint8_t pin;

public:
    DOut(uint8_t PIN);

    void high(void);

    void low(void);

    void toggle(void);
};

#endif //DOUT_H
