//
// Created by Matthijs Oosterhoff on 27/02/16.
//

#ifndef HBRIDGE_DRIVER_LED_H
#define HBRIDGE_DRIVER_LED_H


#include <stdint.h>

class Led {
public:
    /**
     * \brief constructor of LED access class
     * \param PORT to access the LED
     * \param DDR  to set the LED port pin to output
     * \param PIN  to access the LED
     */
    Led(volatile uint8_t *PORT, volatile uint8_t *DDR, uint8_t PIN);

    //! destructor
    ~Led();

    //! switches the LED on
    void on(void);

    //! switches the LED off
    void off(void);

    //! toggles the LED
    void toggle(void);

private:

    //! pointer to the LED port
    volatile uint8_t *m_Port;
    //! pointer to the LED port's data direction register
    volatile uint8_t *m_Ddr;
    //! pin the LED is connected to
    uint8_t m_Pin;
};

#endif //HBRIDGE_DRIVER_LED_H
