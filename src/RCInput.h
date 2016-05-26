//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#ifndef SC_RCINPUT_H
#define SC_RCINPUT_H


#include <avr/io.h>

#define RC_PWM_MIN 1000
#define RC_PWM_MAX 2000
#define RC_PWM_NEUTRAL 1500


void setupRcInput();

uint64_t getCurrentRcInput();

#endif //SC_RCINPUT_H
