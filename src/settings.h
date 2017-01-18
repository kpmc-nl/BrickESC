//
// Created by matthijs on 1/18/17.
//

#ifndef SC_SETTINGS_H
#define SC_SETTINGS_H


#include <stdint.h>

typedef struct {
    uint64_t min_pulse;
    uint64_t max_pulse;
} settings_t;

settings_t get_settings();

void setup_mode();

void write_settins();

void read_settings();

void validate_settings();


#endif /* SC_SETTINGS_H */
