//
// Created by matthijs on 10/03/16.
//

#ifndef SPEED_CONTROLLER_CONTROLLER_H
#define SPEED_CONTROLLER_CONTROLLER_H


#include <output/DOut.h>
#include <input/RCInput.h>
#include <avr/eeprom.h>

#define LOW_THRESH 1420
#define HIGH_THRESH 1580
#define NEUTRAL 1500
#define OUTER_THRESH 20;


typedef struct {
    uint64_t minPulse;
    uint64_t maxPulse;
} Settings;

// settings in eeprom
static Settings EEMEM EESettings;

class Controller {

private:

    RCInput *rcInput;
    DOut *led0;
    DOut *led1;

    // settings in ram
    Settings settings;

    /* used for mapping input pulse to output pulse */
    uint64_t lowDiff;
    uint64_t highDiff;

    /* the current input pulse */
    uint64_t pulse;

public:

    Controller(RCInput *rcInput, DOut *led0, DOut *led1);

    void init();

    void update();

    uint64_t getLowDiff();

    uint64_t getHighDiff();

    uint64_t getPulse();

private:

    void setupMode();

    void writeSettings();

    void readSettings();

    void validateSettings();

    void waitForNeutral();

};


#endif //SPEED_CONTROLLER_CONTROLLER_H
