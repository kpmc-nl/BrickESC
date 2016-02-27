#include "Led.h"

/**
 * \brief constructor of LED access class
 * \param PORT to access the LED
 * \param DDR  to set the LED port pin to output
 * \param PIN  to access the LED
 */
Led::Led(volatile uint8_t *PORT, volatile uint8_t *DDR, uint8_t PIN)
        : m_Port(PORT), m_Ddr(DDR), m_Pin(PIN) {
    *(m_Ddr) |= (1 << m_Pin);
}

//! destructor
Led::~Led() {
}

//! switches the LED on
void Led::on(void) {
    *(m_Port) |= (1 << m_Pin);
}

//! switches the LED off
void Led::off(void) {
    *(m_Port) &= ~(1 << m_Pin);
}

//! toggles the LED
void Led::toggle(void) {
    *(m_Port) ^= (1 << m_Pin);
}