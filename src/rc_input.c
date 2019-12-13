//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#include "rc_input.h"
#include <Arduino.h>
#include "pinout.h"
#include "rc.h"

#define RC_SAMPLE_SIZE 9

static uint64_t buffer[RC_SAMPLE_SIZE];
static uint64_t prev_time = 0;
static uint8_t index = 0;

/* forward declarations */
static void handle_input_falling();

static void handle_input_rising();


/* interface specified by rc_input.h*/
void rc_input_setup() {
    /* input 0 is pin 3 */
    pinMode(RC_input_PIN, INPUT_PULLUP);

    for (int i = 0; i < RC_SAMPLE_SIZE; i++) {
        buffer[i] = RC_PWM_NEUTRAL;
    }
    attachInterrupt(RC_input_INT, handle_input_falling, FALLING);
}

uint64_t rc_input_get_current() {
    /* timeout after 40ms (2* 'healthy' rc-pwm time) */
    if (micros() - prev_time > 40000) {
        return RC_PWM_NEUTRAL;
    }
    uint64_t res = 0;
    for (int i = 0; i < RC_SAMPLE_SIZE; i++) {
        res = res + buffer[i];
    }
    return res / RC_SAMPLE_SIZE;
}


/* impl of 'private' functions */
static void handle_input_falling() {
    attachInterrupt(RC_input_INT, handle_input_rising, RISING);
    prev_time = micros();
}

static void handle_input_rising() {
    attachInterrupt(RC_input_INT, handle_input_falling, FALLING);


    uint64_t pwm_value = micros() - prev_time;

    pwm_value = max(pwm_value, RC_PWM_MIN);
    pwm_value = min(pwm_value, RC_PWM_MAX);

    if (index == RC_SAMPLE_SIZE) {
        index = 0;
    }
    buffer[index] = pwm_value; //( / RC_SAMPLE_SIZE);
    index = index + 1;
}

