//
// Created by matthijs on 10/03/16.
//

#include <util/delay.h>
#include "Controller.h"

Controller::Controller(RCInput *rcInput, DOut *led0, DOut *led1)
        : rcInput(rcInput), led0(led0), led1(led1), pulse(NEUTRAL) {

}

void Controller::setupMode() {
    /* inidcate that we are running the setup mode */

    for (uint8_t i = 0; i < 6; i++) {
        led0->toggle();
        led1->toggle();
        _delay_ms(100);
    }

    /* read max pulse, assuming the user gives max input */
    settings.maxPulse = rcInput->getPulse() - OUTER_THRESH;
    led0->high();

    /* wait for low pulse */
    while (rcInput->getPulse() > LOW_THRESH) {
        _delay_ms(10);
    }
    _delay_ms(500);
    /* read low pulse, assuming the user gives min input */
    settings.minPulse = rcInput->getPulse() + OUTER_THRESH;
    led1->high();
}

void Controller::readSettings() {
    /* read settings from eeprom if controller is started while the signal is neutral or low */
    eeprom_read_block(&settings, &EESettings, sizeof(Settings));

}

void Controller::writeSettings() {
    /* store the settings in the eeprom */
    eeprom_write_block(&settings, &EESettings, sizeof(Settings));
}


void Controller::validateSettings() {
    // Validate settings as much as we can. If any error is detected,
    // the leds will blink furiously, to indicate that the setup must be performed.
    if (settings.minPulse < 500 || settings.minPulse > LOW_THRESH ||
        settings.maxPulse > 2500 || settings.maxPulse < HIGH_THRESH ||
        settings.minPulse > settings.maxPulse) {
        while (1) {
            led1->toggle();
            _delay_ms(100);
            led0->toggle();
            _delay_ms(100);
        }
    }
}

void Controller::waitForNeutral() {
    /* wait for neutral input */
    while (rcInput->getPulse() > HIGH_THRESH || rcInput->getPulse() < LOW_THRESH) {
        _delay_ms(10);
        /* wait for neutral */
    }
    led0->low();
    led1->low();
}

void Controller::init() {
    _delay_ms(500);
    if (rcInput->getPulse() > HIGH_THRESH) {
        /* run setup mode if controller is turned on while the signal is high */
        setupMode();
        writeSettings();
    } else {
        readSettings();
    }

    writeSettings();

    /* initialize our runtime settings */
    highDiff = settings.maxPulse - HIGH_THRESH;
    lowDiff = LOW_THRESH - settings.minPulse;

    validateSettings();
    waitForNeutral();
}

void Controller::update() {
#ifdef LOOPTIME
    uint32_t read_pulse = rcInput->getPulse();

    if ((read_pulse <= LOW_THRESH && pulse > NEUTRAL) || (read_pulse >= HIGH_THRESH && pulse < NEUTRAL)) {
        pulse = NEUTRAL;
        return;
    }

    if (rcInput->getPulse() > pulse) {
        pulse++;
    } else if (rcInput->getPulse() < pulse) {
        pulse--;
    }
#else
    pulse = rcInput->getPulse();
#endif

    if (pulse >= settings.maxPulse) {
        pulse = settings.maxPulse;
        led0->high();
    } else if (pulse <= settings.minPulse) {
        pulse = settings.minPulse;
        led0->high();
    } else {
        led0->low();
    }


}

uint64_t Controller::getHighDiff() {
    return highDiff;
}

uint64_t Controller::getLowDiff() {
    return lowDiff;
}

uint64_t Controller::getPulse() {
    return pulse;
}