/**
 * Firmware for an RC speed controller with a circuit based on some FETs and two relays
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "output/DOut.h"
#include "clock/Clock.h"
#include "output/PWMOut.h"
#include "input/RCInput.h"

#define LOW_THRESH 1420
#define HIGH_THRESH 1580


PWMOut fet(&TCCR0A, &TCCR0B, &OCR0A, &DDRB, PB0, COM0A0, COM0A1);
RCInput rcInput(&PORTB, &PINB, &DDRB, PB2, PCINT2);
DOut relay(&PORTB, &DDRB, PB1);

DOut led0(&PORTB, &DDRB, PB3);
DOut led1(&PORTB, &DDRB, PB4);


typedef struct {
    uint64_t minPulse;
    uint64_t maxPulse;
} Settings;

// settings in eeprom
Settings EEMEM EESettings;
// settings in ram
Settings settings = {1000, 2000};

/* used for mapping input pulse to output pulse */
uint64_t lowDiff;
uint64_t highDiff;

/* the current input pulse */
uint64_t pulse = 1500;

uint64_t update;


int main(void) {
    /* run at 8MHz */
    CLKPR = 0x80; // set system clock to 8mhz with no prescale
    CLKPR = 0x00; // these 2 CLKPR instructions have to be run together in order to set clock to 8 Mhz

    Clock::init();

    _delay_ms(500); // wait for RCInput to parse first signal

    if (rcInput.getPulse() > HIGH_THRESH) {
        /* run setup mode if controller is turned on while the signal is high */

        /* inidcate that we are running the setup mode */
        for (uint8_t i = 0; i < 6; i++) {
            led0.toggle();
            led1.toggle();
            _delay_ms(100);
        }

        /* read max pulse, assuming the user gives max input */
        settings.maxPulse = rcInput.getPulse();
        led0.high();

        /* wait for low pulse */
        while (rcInput.getPulse() > LOW_THRESH) {
            _delay_ms(10);
        }
        _delay_ms(500);
        /* read low pulse, assuming the user gives min input */
        settings.minPulse = rcInput.getPulse();
        led1.high();

        /* store the settings in the eeprom */
        eeprom_write_block(&settings, &EESettings, sizeof(Settings));

    } else {
        /* read settings from eeprom if controller is started while the signal is neutral or low */
        eeprom_read_block(&settings, &EESettings, sizeof(Settings));
    }

    /* initialize our runtime settings */
    highDiff = settings.maxPulse - HIGH_THRESH;
    lowDiff = LOW_THRESH - settings.minPulse;



    // Validate settings as much as we can. If any error is detected,
    // the leds will blink furiously, to indicate that the setup must be performed.
    if (settings.minPulse < 500 || settings.minPulse > LOW_THRESH ||
        settings.maxPulse > 2500 || settings.maxPulse < HIGH_THRESH ||
        settings.minPulse > settings.maxPulse) {
        while (1) {
            led1.toggle();
            _delay_ms(100);
            led0.toggle();
            _delay_ms(100);
        }
    }

    /* wait for neutral input */
    while (rcInput.getPulse() > HIGH_THRESH || rcInput.getPulse() < LOW_THRESH) {
        _delay_ms(10);
        /* wait for neutral */
    }
    led0.low();
    led1.low();

    /* if everything is initialized, run the actual ESC logic */
    while (1) {

#ifdef SLOW_RESPONSE_MODE
        if (Clock::millis() - update < 3) {
            continue;
        }
        update = Clock::millis();

        if (rcInput.getPulse() > pulse) {
            pulse++;
        } else if (rcInput.getPulse() < pulse) {
            pulse--;
        }
#else
        pulse = rcInput.getPulse();
#endif
        if (pulse > LOW_THRESH && pulse < HIGH_THRESH) {
            fet.write(0);
            led1.high();
            continue;
        }
        led1.low();


        if (pulse >= HIGH_THRESH) {
            pulse = pulse > settings.maxPulse ? settings.maxPulse : pulse;
            pulse == settings.maxPulse ? led0.high() : led0.low();
            relay.low();
            fet.write((pulse - HIGH_THRESH) * 255 / highDiff);
            continue;
        }

        if (pulse <= LOW_THRESH) {
            pulse = pulse < settings.minPulse ? settings.minPulse : pulse;
            pulse == settings.minPulse ? led0.high() : led0.low();
            relay.high();
            fet.write((LOW_THRESH - pulse) * 255 / lowDiff);
            continue;
        }

    }
}

