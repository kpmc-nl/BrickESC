//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#include <wiring.h>
#include "RCInput.h"
#include "util.h"

static uint64_t pulse_length_sample[RC_SAMPLE_SIZE];
static uint64_t prev_time = 0;
static uint8_t sample_counter;

static void input_rising();

static void input_falling();

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

void setupRcInput() {
    attachInterrupt(0, input_rising, RISING);
}

uint64_t getCurrentRcInput() {
    /* timeout after 40ms (2* 'healthy' rc-pwm time) */
    if (micros() - prev_time > 40000) {
        return RC_PWM_NEUTRAL;
    }
    uint32_t result = median(pulse_length_sample, RC_SAMPLE_SIZE);
    return result;
}