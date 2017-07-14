//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#include "controller.h"
#include <Arduino.h>
#include "pinout.h"
#include "rc.h"
#include "util.h"
#include "motor.h"
#include "rc_input.h"
#include "settings.h"

#define LOOPTIME 3000

static uint64_t target_pulse = RC_PWM_MIN;
static int cutoff_voltage = 0;
static int cutoff_temperature = 40; // C // NTC B57164-K103-K
static boolean reduce_power = false;


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
        write_settins();
    } else {
        read_settings();
        delay(500);
    }

    validate_settings();


    for (int8_t i = 1; i <= cell_count; i++) {
        delay(800);
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED2_PIN, HIGH);
        motor_tone(1500, 60);
        digitalWrite(LED1_PIN, LOW);
        digitalWrite(LED2_PIN, LOW);
    }

}

void controller_loop() {

    for (uint8_t i = 0; get_battery_voltage() < cutoff_voltage && i < 10; i++) {
        if (i == 9) {
            reduce_power = true;
        }
    }

    for (uint8_t i = 0;
         get_temperature_voltage() < ((-0.009501682077898 * cutoff_temperature + 0.98204925041399) * 5000)
         && i < 10; i++) {
        if (i == 9) {
            reduce_power = true;
//            digitalWrite(LED2_PIN, HIGH);
        }
    }



    target_pulse = rc_input_get_current();

    if(target_pulse >= get_settings().max_pulse){
        digitalWrite(LED1_PIN, HIGH);
        analogWrite(FET_PIN, 255);
        return;
    }

    if(target_pulse <= get_settings().min_pulse){
        analogWrite(FET_PIN, 0);
        digitalWrite(LED2_PIN, HIGH);
        return;
    }


    if (target_pulse <= get_settings().min_pulse || target_pulse >= get_settings().max_pulse) {
        digitalWrite(LED1_PIN, HIGH);
    } else {
        digitalWrite(LED1_PIN, LOW);
    }

    uint8_t full_power = 255;
    if (reduce_power) {
        full_power = 64;
    }

    analogWrite(FET_PIN, (target_pulse - get_settings().min_pulse) * full_power / (get_settings().max_pulse - get_settings().min_pulse));

}





int get_battery_voltage() {
    /* in millivolts */
    return map(analogRead(VOLTAGE_SENSOR), 0, 1023, 0, 21000);
}

int get_temperature_voltage() {
    /* in millivolts */
    return map(analogRead(TEMP_SENSOR), 0, 1023, 0, 5000);
}
