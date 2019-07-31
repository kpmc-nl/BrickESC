//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#include "controller.h"
#include <Arduino.h>
#include "pinout.h"
#include "rc.h"
#include "util.h"
#include "motor.h"
#include "input.h"
#include "settings.h"

#define LOOPTIME 1000

static int target_pulse = RC_PWM_NEUTRAL;

static unsigned long last_motor_on;


void controller_setup() {
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(FET_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);

    delay(500);


    motor_tone(600, 200);
    motor_tone(900, 200);
    motor_tone(1200, 200);

    if (input_get_current() > RC_PWM_HIGH_THRESH) {
        /* run setup mode if controller is turned on while the signal is high */
        setup_mode();
        write_settins();
    } else {
        read_settings();
        delay(500);
    }

    validate_settings();
    wait_for_neutral();


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

    delay(20);


    delayMicroseconds(LOOPTIME);

    uint64_t pulse = input_get_current();

    if(pulse > target_pulse){

        if(target_pulse < RC_PWM_NEUTRAL && pulse >= RC_PWM_LOW_THRESH){
            target_pulse = min(RC_PWM_NEUTRAL, target_pulse + 15);
        }else{
            target_pulse++;
        }

    }else{

        if(target_pulse > RC_PWM_NEUTRAL && pulse <= RC_PWM_HIGH_THRESH){
            target_pulse = max(RC_PWM_NEUTRAL, target_pulse - 15);
        }else{
            target_pulse--;
        }

    }

    if (target_pulse > RC_PWM_LOW_THRESH && target_pulse < RC_PWM_HIGH_THRESH) {
        digitalWrite(FET_PIN, LOW);
        digitalWrite(LED2_PIN, HIGH);
        digitalWrite(LED1_PIN, LOW);

        if(millis() - 3000 > last_motor_on){
            /* make sure the relay is not active when throttle is idle */
            motor_forward();
        }
        return;
    }else{
        last_motor_on = millis();
    }

    digitalWrite(LED2_PIN, LOW);

    if (target_pulse <= get_settings().min_pulse || target_pulse >= get_settings().max_pulse) {
        digitalWrite(LED1_PIN, HIGH);
    } else {
        digitalWrite(LED1_PIN, LOW);
    }



    if (target_pulse >= RC_PWM_HIGH_THRESH) {
        motor_forward();
        motor_power((target_pulse - RC_PWM_HIGH_THRESH) * 255 / (get_settings().max_pulse - RC_PWM_HIGH_THRESH));
        return;
    }

    if (target_pulse <= RC_PWM_LOW_THRESH) {
        motor_reverse();
        motor_power((RC_PWM_LOW_THRESH - target_pulse) * 255 / (RC_PWM_LOW_THRESH - get_settings().min_pulse));
        return;
    }

}


void wait_for_neutral() {
    /* wait for neutral input */
    while (input_get_current() > RC_PWM_HIGH_THRESH || input_get_current() < RC_PWM_LOW_THRESH) {
        delay(10);
    }
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
}


int rc_get_input_current() {
    return analogRead(INPUT_ADC);
}