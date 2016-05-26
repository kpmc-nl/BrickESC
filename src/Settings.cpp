//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#include <Arduino.h>
#include "Settings.h"
#include "RCInput.h"
#include "Controller.h"


void Settings::setupMode() {
    /* inidcate that we are running the setup mode */

    for (uint8_t i = 0; i < 3; i++) {
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED2_PIN, HIGH);
        delay(100);
        digitalWrite(LED1_PIN, LOW);
        digitalWrite(LED2_PIN, LOW);
        delay(100);
    }

    /* read max pulse, assuming the user gives max input */
    data.maxPulse = getCurrentRcInput() - RC_PWM_OUTER_THRESH;
    digitalWrite(LED1_PIN, HIGH);

    /* wait for low pulse */
    while (getCurrentRcInput() > RC_PWM_LOW_THRESH) {
        delay(10);
    }
    delay(500);
    /* read low pulse, assuming the user gives min input */
    data.minPulse = getCurrentRcInput() + RC_PWM_OUTER_THRESH;
    digitalWrite(LED2_PIN, HIGH);
}

void Settings::readSettings() {
    /* read settings from eeprom if controller is started while the signal is neutral or low */
    eeprom_read_block(&data, &eeData, sizeof(SettingsData));

}

void Settings::writeSettings() {
    /* store the settings in the eeprom */
    eeprom_write_block(&data, &eeData, sizeof(SettingsData));
}


void Settings::validateSettings() {
    // Validate settings as much as we can. If any error is detected,
    // the leds will blink furiously, to indicate that the setup must be performed.
    if (data.minPulse < 500 || data.minPulse > RC_PWM_LOW_THRESH ||
        data.maxPulse > 2500 || data.maxPulse < RC_PWM_HIGH_THRESH ||
        data.minPulse > data.maxPulse) {
        while (1) {
            digitalWrite(LED1_PIN, HIGH);
            digitalWrite(LED2_PIN, LOW);
            delay(100);
            digitalWrite(LED2_PIN, HIGH);
            digitalWrite(LED1_PIN, LOW);
            delay(100);
        }
    }
}

void Settings::waitForNeutral() {
    /* wait for neutral input */
    while (getCurrentRcInput() > RC_PWM_HIGH_THRESH || getCurrentRcInput() < RC_PWM_LOW_THRESH) {
        delay(10);
        /* wait for neutral */
    }
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}

void Settings::init() {
    delay(500);
    if (getCurrentRcInput() > RC_PWM_HIGH_THRESH) {
        /* run setup mode if controller is turned on while the signal is high */
        setupMode();
        writeSettings();
    } else {
        readSettings();
    }

    writeSettings();

    /* initialize our runtime settings */
    highDiff = data.maxPulse - RC_PWM_HIGH_THRESH;
    lowDiff = RC_PWM_LOW_THRESH - data.minPulse;

    validateSettings();
    waitForNeutral();
}


uint64_t Settings::getHighDiff() {
    return highDiff;
}

uint64_t Settings::getLowDiff() {
    return lowDiff;
}


uint64_t Settings::getMinPulse() {
    return data.minPulse;
}

uint64_t Settings::getMaxPulse() {
    return data.maxPulse;
}