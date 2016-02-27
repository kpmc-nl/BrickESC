#ifndef DOUT_H
#define DOUT_H

#include <stdint.h>

/**
 * \brief Wrapper around digital output.
 */
class DOut {

private:

    volatile uint8_t *port;
    volatile uint8_t *ddr;
    uint8_t pin;

public:
    /**
     * \brief constructor for DOut class.
     *
     * \param PORT pointer to the output port
     * \param DDR  pointer to the output port's data direction register
     * \param PIN  pin the output is connected to
     */
    DOut(volatile uint8_t *PORT, volatile uint8_t *DDR, uint8_t PIN);

    //! destructor
    ~DOut();

    //! switches output to high state
    void high(void);

    //! switches the output to low state
    void low(void);

    //! toggles the output
    void toggle(void);


};

#endif //DOUT_H
