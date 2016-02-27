#include "DOut.h"

DOut::DOut(volatile uint8_t *PORT, volatile uint8_t *DDR, uint8_t PIN)
        : port(PORT), ddr(DDR), pin(PIN) {
    /* Set output pin direction */
    *(ddr) |= (1 << pin);
}

DOut::~DOut() {
}

void DOut::high(void) {
    *(port) |= (1 << pin);
}

void DOut::low(void) {
    *(port) &= ~(1 << pin);
}

void DOut::toggle(void) {
    *(port) ^= (1 << pin);
}