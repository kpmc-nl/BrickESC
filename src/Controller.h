//
// Created by Matthijs Oosterhoff on 26/05/16.
//

#ifndef SC_CONTROLLER_H
#define SC_CONTROLLER_H

#define LED1_PIN PIN_A6
#define LED2_PIN PIN_B0
#define FET_PIN PIN_A7
#define RELAY_PIN PIN_B2
#define VOLTAGE_SENSOR ADC_Input_ADC1

#define RC_PWM_MIN 1000
#define RC_PWM_MAX 2000
#define RC_PWM_NEUTRAL 1500
#define RC_PWM_LOW_THRESH 1420
#define RC_PWM_HIGH_THRESH 1580
#define RC_PWM_OUTER_THRESH 20


#define RC_SAMPLE_SIZE 11
#define LOOPTIME 1500

#include "Settings.h"
#include "RCInput.h"

class Controller {

private:
    uint64_t target_pulse;
    bool battery_dead;
    int cutoff_voltage;

    int getBatteryVoltage();

public:
    Controller();

    void setup();

    void loop();

    void motorTone(uint64_t freq, uint64_t duration);

};


#endif //SC_CONTROLLER_H
