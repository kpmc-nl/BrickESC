//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#include "rc_input.h"
#include <Arduino.h>
#include "pinout.h"
#include "rc.h"
#include "util.h"

#define RC_SAMPLE_SIZE 11

static uint64_t pulse_length_sample[RC_SAMPLE_SIZE];
static uint64_t prev_time = 0;
static uint8_t sample_counter;

/* forward declarations */
static void handle_input_falling();

static void hanle_input_rising();


/* interface specified by rc_input.h*/
void rc_input_setup() {
    /* input 0 is pin 3 */
    pinMode(RC_input_PIN,INPUT_PULLUP);
    attachInterrupt(RC_input_INT, handle_input_falling, FALLING);
}

uint64_t rc_input_get_current() {
    /* timeout after 40ms (2* 'healthy' rc-pwm time) */
    if (micros() - prev_time > 40000) {
        return RC_PWM_NEUTRAL;
    }
    uint32_t result = median(pulse_length_sample, RC_SAMPLE_SIZE);
    return result;
}


/* impl of 'private' functions */
static void handle_input_falling() {
    attachInterrupt(RC_input_INT, hanle_input_rising, RISING);
    prev_time = micros();
}

static void hanle_input_rising() {
    attachInterrupt(RC_input_INT, handle_input_falling, FALLING);


    uint64_t pwm_value = micros() - prev_time;

    pwm_value = max(pwm_value, RC_PWM_MIN);
    pwm_value = min(pwm_value, RC_PWM_MAX);

    if (sample_counter == RC_SAMPLE_SIZE) {
        sample_counter = 0;
    }
    pulse_length_sample[sample_counter] = pwm_value;
    ++sample_counter;

}

