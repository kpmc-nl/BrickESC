//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#include "motor.h"
#include <Arduino.h>
#include "pinout.h"


void motor_setup() {
    pinMode(FET_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);


    /* Set WGM (Wave Form Generation Mode) to Fast PWM */
    TCCR0A |= (1 << WGM00) | (1 << WGM01);

    /* Set CS (Clock Select) to internal clock 0; IE no prescaling */
    TCCR0B |= (1 << CS00);

}


void motor_power(int power) {
    analogWrite(FET_PIN, power);
}

void motor_reverse() {
    digitalWrite(RELAY_PIN, HIGH);
}

void motor_forward() {
    digitalWrite(RELAY_PIN, LOW);
}

void motor_tone(uint64_t freq, uint64_t duration) {
    uint64_t period_usec = 1000000 / freq;
    uint64_t k = duration * 1000 / period_usec;

    for (uint64_t i = 0; i < k; i++) {
        digitalWrite(FET_PIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(FET_PIN, LOW);
        delayMicroseconds(period_usec);
    }
}
