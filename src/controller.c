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

static uint64_t target_pulse = 1500;

static uint64_t last_motor_on = 0;

static uint64_t rc_pwm_neutral;
static uint64_t rc_pwm_low_thresh;
static uint64_t rc_pwm_high_thresh;

void controller_setup() {
    pinMode(FORWARD_PIN, OUTPUT);
    pinMode(REVERSE_PIN, OUTPUT);

    delay(500);

    motor_tone(600, 200);
    motor_tone(900, 200);
    motor_tone(1200, 200);

    if (rc_input_get_current() > RC_PWM_NEUTRAL + 200) {
        /* run setup mode if controller is turned on while the signal is high */
        setup_mode();
        write_settins();
    } else {
        read_settings();
        delay(500);
    }

    validate_settings();

    rc_pwm_neutral = (get_settings().max_pulse + get_settings().min_pulse) / 2;
    rc_pwm_high_thresh = rc_pwm_neutral + RC_PWM_INNER_THRESH;
    rc_pwm_low_thresh = rc_pwm_neutral - RC_PWM_INNER_THRESH;

    wait_for_neutral();

    for (int8_t i = 1; i <= 2; i++) {
        delay(800);
        motor_tone(600, 60);
    }

}

void controller_loop() {

    uint64_t pulse = rc_input_get_current();

    if (pulse > target_pulse) {

        if (target_pulse < rc_pwm_neutral && pulse >= rc_pwm_low_thresh) {
            target_pulse = min(rc_pwm_neutral, target_pulse + 15);
        } else {
            target_pulse++;
        }

    } else {

        if (target_pulse > rc_pwm_neutral && pulse <= rc_pwm_high_thresh) {
            target_pulse = max(rc_pwm_neutral, target_pulse - 15);
        } else {
            target_pulse--;
        }

    }

    if (target_pulse > rc_pwm_low_thresh && target_pulse < rc_pwm_high_thresh) {
        digitalWrite(FORWARD_PIN, LOW);

        if (millis() - 3000 > last_motor_on) {
            /* make sure the relay is not active when throttle is idle */
            motor_forward();
        }
        return;
    } else {
        last_motor_on = millis();
    }

    uint8_t full_power = 255;

    // if target_pulse is set for forward
    if (target_pulse >= rc_pwm_high_thresh) {
        motor_forward();
        motor_power((target_pulse - rc_pwm_high_thresh) * full_power / (get_settings().max_pulse - rc_pwm_high_thresh));
        return;
    }
    
    // if target_pulse is set for reverse
    if (target_pulse <= rc_pwm_low_thresh) {
        motor_reverse();
        motor_power((rc_pwm_low_thresh - target_pulse) * full_power / (rc_pwm_low_thresh - get_settings().min_pulse));
        return;
    }
    
    // if target pulse is between forward and reverse
    if ((target_pulse < rc_pwm_high_thresh) && (target_pulse > rc_pwm_low_thresh)) {
        motor_stop();
    }
}


void wait_for_neutral() {
    /* wait for neutral input */
    while (rc_input_get_current() > rc_pwm_high_thresh || rc_input_get_current() < rc_pwm_low_thresh) {
        delay(10);
    }
}
