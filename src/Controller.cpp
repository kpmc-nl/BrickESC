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

    target_pulse = getCurrentRcInput();


    analogWrite(FET_PIN, (target_pulse - settings.getMinPulse()) * 255 / (settings.getMaxPulse() - settings.getMinPulse()));

}