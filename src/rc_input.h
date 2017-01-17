//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#ifndef RC_INPUT_H
#define RC_INPUT_H


#include <avr/io.h>


void rc_input_setup();

uint64_t rc_input_get_current();

#endif /* RC_INPUT_H */
