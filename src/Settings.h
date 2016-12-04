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


public:

    void init();

    uint64_t getMaxPulse();

    uint64_t getMinPulse();

private:

    void setupMode();

    void writeSettings();

    void readSettings();

    void validateSettings();

    void motorTone(uint64_t freq, uint64_t duration);

};


#endif //SC_SETTINGS_H
