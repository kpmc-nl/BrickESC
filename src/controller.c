//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#include "controller.h"
#include <Arduino.h>
#include "pinout.h"
#include "rc.h"
#include "motor.h"
#include "rc_input.h"
#include "settings.h"


static uint64_t target_pulse = RC_PWM_NEUTRAL;


void controller_setup() {
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(FET_PIN, OUTPUT);

    delay(500);

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

    for (int8_t i = 1; i <= 2; i++) {
        delay(800);
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED2_PIN, HIGH);
        motor_tone(1500, 60);
        digitalWrite(LED1_PIN, LOW);
        digitalWrite(LED2_PIN, LOW);
    }

}

void controller_loop() {


    target_pulse = rc_input_get_current();

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

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

    analogWrite(FET_PIN, (target_pulse - get_settings().min_pulse) * 255 / (get_settings().max_pulse - get_settings().min_pulse));
}


