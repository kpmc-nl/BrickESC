#include <avr/io.h>
#include <util/delay.h>
#include "Led.h"

/**
 * \brief main loop
 */
int main(void) {
    Led led(&PORTB, &DDRB, PINB0);

    while (1) {
        led.toggle();
        _delay_ms(500);
    }
}