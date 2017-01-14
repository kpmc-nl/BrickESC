//
// Created by Matthijs Oosterhoff on 26/05/16.
//

#include <Arduino.h>
#include <PwmTimer.h>
#include "Controller.h"
#include "core_adc.h"
#include "main.h"

Controller::Controller() {
    target_pulse = RC_PWM_NEUTRAL;
    battery_dead = false;
}

void Controller::setup() {


    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(FET_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);

    Timer2_SetWaveformGenerationMode(Timer2_Fast_PWM_FF);
    Timer2_ClockSelect(Timer2_Prescale_Value_1);


    int voltage = getBatteryVoltage();
    uint8_t cell_count = 1 + (voltage / 4250);
    cutoff_voltage = 3400 * cell_count;

    setupRcInput();
    settings.init();


    for(uint8_t i=0; i<cell_count; i++){
        delay(100);
        digitalWrite(LED1_PIN, HIGH);
        motorTone(600, 200);
        digitalWrite(LED1_PIN, LOW);
    }

}

int Controller::getBatteryVoltage() {
    /* in millivolts */
    return map(analogRead(VOLTAGE_SENSOR), 0, 1023, 0, 21000);
}

void Controller::loop() {

    for(uint8_t i=0; getBatteryVoltage() < cutoff_voltage && i < 5; i++){
        if(i==4){
            battery_dead = true;
        }
    }


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
//        digitalWrite(RELAY_PIN, LOW);
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

    uint8_t full_power = 255;
    if(battery_dead){
        full_power = 48;
    }

    if (target_pulse >= RC_PWM_HIGH_THRESH) {
        digitalWrite(RELAY_PIN, LOW);
        analogWrite(FET_PIN, (target_pulse - RC_PWM_HIGH_THRESH) * full_power / settings.getHighDiff());
        return;
    }

    if (target_pulse <= RC_PWM_LOW_THRESH) {
        digitalWrite(RELAY_PIN, HIGH);
        analogWrite(FET_PIN, (RC_PWM_LOW_THRESH - target_pulse) * full_power / settings.getLowDiff());
        return;
    }
}

void Controller::motorTone(uint64_t freq, uint64_t duration) {
    uint64_t period_usec = 1000000 / freq;
    uint64_t k = duration * 1000 / period_usec;

    for (uint64_t i = 0; i < k; i++) {
        digitalWrite(FET_PIN, HIGH);
        delayMicroseconds(1);
        digitalWrite(FET_PIN, LOW);
        delayMicroseconds(period_usec);
    }
}