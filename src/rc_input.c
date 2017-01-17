//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#include <wiring.h>
#include "rc.h"
#include "rc_input.h"
#include "util.h"

#define RC_SAMPLE_SIZE 11

static uint64_t pulse_length_sample[RC_SAMPLE_SIZE];
static uint64_t prev_time = 0;
static uint8_t sample_counter;

/* forward declarations */
static void input_rising();

static void input_falling();

/* interface specified by rc_input.h*/
void rc_input_setup() {
    /* input 0 is pin 3 */
    attachInterrupt(0, input_rising, RISING);
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
static void input_rising() {
    attachInterrupt(0, input_falling, FALLING);
    prev_time = micros();
}

static void input_falling() {
    attachInterrupt(0, input_rising, RISING);


    uint64_t pwm_value = micros() - prev_time;

    pwm_value = max(pwm_value, RC_PWM_MIN);
    pwm_value = min(pwm_value, RC_PWM_MAX);

    if (sample_counter == RC_SAMPLE_SIZE) {
        sample_counter = 0;
    }
    pulse_length_sample[sample_counter] = pwm_value;
    ++sample_counter;

}

