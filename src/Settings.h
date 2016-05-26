//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#ifndef SC_SETTINGS_H
#define SC_SETTINGS_H

#include <avr/eeprom.h>




typedef struct {
    uint64_t minPulse;
    uint64_t maxPulse;
} SettingsData;

// settings in eeprom
static SettingsData EEMEM eeData;

class Settings {

private:
    // settingsdata in ram
    SettingsData data;

    /* used for mapping input pulse to output pulse */
    uint64_t lowDiff;
    uint64_t highDiff;

public:

    void init();

    uint64_t getLowDiff();

    uint64_t getHighDiff();

    uint64_t getMaxPulse();

    uint64_t getMinPulse();

private:

    void setupMode();

    void writeSettings();

    void readSettings();

    void validateSettings();

    void waitForNeutral();

};


#endif //SC_SETTINGS_H
