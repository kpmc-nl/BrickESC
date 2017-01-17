//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#include <wiring.h>
#include <core_pins.h>
#include <core_adc.h>
#include <avr/eeprom.h>
#include "controller.h"
#include "pinout.h"
#include "motor.h"
#include "rc_input.h"
#include "rc.h"
#include "util.h"


#define LOOPTIME 1500

typedef struct {
    uint64_t min_pulse;
    uint64_t max_pulse;
} settings_t;

settings_t EEMEM eeprom_settings;
settings_t runtime_settings;


/* predeclarations */
static void validate_settings();

static void setup_mode();

static void wait_for_neutral();

static int get_battery_voltage();

void write_settings();

static int cutoff_voltage = 0;
static boolean battery_dead = false;
static uint64_t target_pulse = RC_PWM_NEUTRAL;


void controller_setup() {
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(FET_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);

    delay(500);

    int voltage = get_battery_voltage();
    uint8_t cell_count = 1 + (voltage / 4250);
    cutoff_voltage = 3400 * cell_count;

    motor_tone(600, 200);
    motor_tone(900, 200);
    motor_tone(1200, 200);

    if (rc_input_get_current() > RC_PWM_HIGH_THRESH) {
        /* run setup mode if controller is turned on while the signal is high */
        setup_mode();
        write_settings();
    } else {
        read_settings();
        delay(500);
    }

    validate_settings();
    wait_for_neutral();



//    high_diff = settings.max_pulse - RC_PWM_HIGH_THRESH;
//    low_diff = RC_PWM_LOW_THRESH - settings.min_pulse;


    for (uint8_t i = 0; i < cell_count; i++) {
        delay(100);
        digitalWrite(LED1_PIN, HIGH);
        motor_tone(600, 200);
        digitalWrite(LED1_PIN, LOW);
    }

}

void controller_loop() {

//    for (uint8_t i = 0; get_battery_voltage() < cutoff_voltage && i < 5; i++) {
//        if (i == 4) {
//            battery_dead = true;
//        }
//    }

    // delayMicroseconds(LOOPTIME);
    //   uint64_t pulse = rc_input_get_current();

//    if ((pulse <= RC_PWM_LOW_THRESH && target_pulse > RC_PWM_NEUTRAL) ||
//        (pulse >= RC_PWM_HIGH_THRESH && target_pulse < RC_PWM_NEUTRAL)) {
//        target_pulse = RC_PWM_NEUTRAL;
//        return;
//    } else if (pulse > target_pulse) {
//        target_pulse++;
//    } else if (pulse < target_pulse) {
//        target_pulse--;
//    }


    target_pulse = rc_input_get_current();

    if (target_pulse > RC_PWM_LOW_THRESH && target_pulse < RC_PWM_HIGH_THRESH) {
        digitalWrite(FET_PIN, LOW);
        digitalWrite(LED2_PIN, HIGH);
        digitalWrite(LED1_PIN, LOW);
        return;
    }
    digitalWrite(LED2_PIN, LOW);

    if (target_pulse <= runtime_settings.min_pulse || target_pulse >= runtime_settings.max_pulse) {
        digitalWrite(LED1_PIN, HIGH);
    } else {
        digitalWrite(LED1_PIN, LOW);
    }

    uint8_t full_power = 255;
//    if (battery_dead) {
//        full_power = 48;
//    }

    if (target_pulse >= RC_PWM_HIGH_THRESH) {
        motor_forward();

        analogWrite(FET_PIN, map((target_pulse - RC_PWM_NEUTRAL), 0, runtime_settings.max_pulse, 0, 255));


        //motor_power((target_pulse - RC_PWM_HIGH_THRESH) * full_power / high_diff);
        return;
    }

    if (target_pulse <= RC_PWM_LOW_THRESH) {
        motor_reverse();
        motor_power(0);

        //motor_power((RC_PWM_LOW_THRESH - target_pulse) * full_power / low_diff);
        return;
    }

}


static void setup_mode() {

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
    runtime_settings.max_pulse = rc_input_get_current() + RC_PWM_OUTER_THRESH;
    digitalWrite(LED2_PIN, HIGH);
    motor_tone(1200, 300);

}

void validate_settings() {
    // Validate settings as much as we can. If any error is detected,
    // the leds will blink furiously, to indicate that the setup must be performed.
    if (runtime_settings.min_pulse < 500 || runtime_settings.min_pulse > RC_PWM_LOW_THRESH ||
        runtime_settings.max_pulse > 2500 || runtime_settings.max_pulse < RC_PWM_HIGH_THRESH ||
        runtime_settings.min_pulse > runtime_settings.max_pulse) {
        while (rc_input_get_current() < RC_PWM_HIGH_THRESH) {

            digitalWrite(LED1_PIN, HIGH);
            digitalWrite(LED2_PIN, LOW);
            motor_tone(900, 100);
            delay(100);
            digitalWrite(LED2_PIN, HIGH);
            digitalWrite(LED1_PIN, LOW);
            motor_tone(900, 100);
            delay(100);
        }
        setup_mode();
        write_settings();
    }
}


void wait_for_neutral() {
    /* wait for neutral input */
    while (rc_input_get_current() > RC_PWM_HIGH_THRESH || rc_input_get_current() < RC_PWM_LOW_THRESH) {
        delay(10);
    }
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}

int get_battery_voltage() {
    /* in millivolts */
    return map(analogRead(VOLTAGE_SENSOR), 0, 1023, 0, 21000);
}

void read_settings() {
    /* read settings from eeprom if controller is started while the signal is neutral or low */
    eeprom_read_block(&runtime_settings, &eeprom_settings, sizeof(settings_t));
}

void write_settings() {
    /* store the settings in the eeprom */
    eeprom_write_block(&runtime_settings, &eeprom_settings, sizeof(settings_t));
}

