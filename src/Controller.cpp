//
// Created by Matthijs Oosterhoff on 26/05/16.
//

#include <Arduino.h>
#include <PwmTimer.h>
#include <avr/power.h>
#include "Controller.h"

Controller::Controller() {
    target_pulse = RC_PWM_NEUTRAL;
}

void Controller::setup() {
    clock_prescale_set(clock_div_1);

    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(FET_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);

    Timer2_SetWaveformGenerationMode(Timer2_Fast_PWM_FF);
    Timer2_ClockSelect(Timer2_Prescale_Value_1);

    setupRcInput();
    settings.init();
}

void Controller::loop() {
#ifdef LOOPTIME
    delayMicroseconds(LOOPTIME);
    uint64_t pulse = getCurrentRcInput();

    if ((pulse <= RC_PWM_LOW_THRESH && target_pulse > RC_PWM_NEUTRAL) || (pulse >= RC_PWM_HIGH_THRESH && target_pulse < RC_PWM_NEUTRAL)) {
        target_pulse = RC_PWM_NEUTRAL;
        return;
    } else if (pulse > target_pulse) {
        target_pulse++;
    } else if (pulse < target_pulse) {
        target_pulse--;
    }
#else
    target_pulse = getCurrentRcInput();
#endif


    if (target_pulse > RC_PWM_LOW_THRESH && target_pulse < RC_PWM_HIGH_THRESH) {
        digitalWrite(FET_PIN, LOW);
        digitalWrite(LED2_PIN, HIGH);
        digitalWrite(LED1_PIN, LOW);
        return;
    }
    digitalWrite(LED2_PIN, LOW);

    if (target_pulse <= settings.getMinPulse() || target_pulse >= settings.getMaxPulse()) {
        digitalWrite(LED1_PIN, HIGH);
    } else {
        digitalWrite(LED1_PIN, LOW);
    }


    if (target_pulse >= RC_PWM_HIGH_THRESH) {
        digitalWrite(RELAY_PIN, LOW);
        analogWrite(FET_PIN, (target_pulse - RC_PWM_HIGH_THRESH) * 255 / settings.getHighDiff());
        return;
    }

    if (target_pulse <= RC_PWM_LOW_THRESH) {
        digitalWrite(RELAY_PIN, HIGH);
        analogWrite(FET_PIN, (RC_PWM_LOW_THRESH - target_pulse) * 255 / settings.getLowDiff());
        return;
    }
}