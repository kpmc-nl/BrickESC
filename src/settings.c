//
// Created by matthijs on 1/18/17.
//


#include "settings.h"
#include <Arduino.h>
#include <avr/eeprom.h>
#include "pinout.h"
#include "rc.h"
#include "motor.h"
#include "rc_input.h"


static settings_t EEMEM eeprom_settings;
static settings_t runtime_settings;


settings_t get_settings() {
    return runtime_settings;
}


void read_settings() {
    eeprom_read_block(&runtime_settings, &eeprom_settings, sizeof(settings_t));

}

void write_settins() {
    eeprom_write_block(&runtime_settings, &eeprom_settings, sizeof(settings_t));
}


void setup_mode() {
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
    runtime_settings.max_pulse = rc_input_get_current() - RC_PWM_OUTER_THRESH;
    digitalWrite(LED1_PIN, HIGH);
    motor_tone(1200, 300);

    /* wait for low pulse */
    while (rc_input_get_current() > RC_PWM_LOW_THRESH) {
        delay(10);
    }
    delay(500);
    /* read low pulse, assuming the user gives min input */
    runtime_settings.min_pulse = rc_input_get_current() + RC_PWM_OUTER_THRESH;
    digitalWrite(LED2_PIN, HIGH);
    motor_tone(1200, 300);

}

void validate_settings() {

    // Validate settings as much as we can. If any error is detected,
    // the leds will blink furiously, to indicate that the setup must be performed.
    if (runtime_settings.min_pulse < 500 || runtime_settings.min_pulse > RC_PWM_LOW_THRESH ||
        runtime_settings.max_pulse > 2500 || runtime_settings.max_pulse < RC_PWM_HIGH_THRESH ||
        runtime_settings.min_pulse > runtime_settings.max_pulse) {
        while (true) {

            digitalWrite(LED1_PIN, HIGH);
            digitalWrite(LED2_PIN, LOW);
            motor_tone(900, 100);
            delay(100);
            digitalWrite(LED2_PIN, HIGH);
            digitalWrite(LED1_PIN, LOW);
            motor_tone(900, 100);
            delay(100);
        }
    }
}