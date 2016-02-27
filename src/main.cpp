//#include <avr/io.h>
//#include <util/delay.h>
//#include "Led.h"

/**
 * \brief main loop
 */
//int main(void) {
//    Led led(&PORTB, &DDRB, PINB0);
//
//    while (1) {
//        led.toggle();
//        _delay_ms(500);
//    }
//}

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRB   |= (1 << PB0);                   // PWM output on PB0
    TCCR0A = (1 << COM0A1) | (1 << WGM00);  // phase correct PWM mode
    OCR0A  = 0x10;                          // initial PWM pulse width

    TCCR0B = (1 << CS01);   // clock source = CLK/8, start PWM


    uint8_t brightness;

    
    while(1)
    {
        // increasing brightness
        for (brightness = 0; brightness < 255; ++brightness)
        {
            // set the brightness as duty cycle
            OCR0A = brightness;

            // delay so as to make the user "see" the change in brightness
            _delay_ms(10);
        }

        // decreasing brightness
        for (brightness = 255; brightness > 0; --brightness)
        {
            // set the brightness as duty cycle
            OCR0A = brightness;

            // delay so as to make the user "see" the change in brightness
            _delay_ms(10);
        }

    }
}
