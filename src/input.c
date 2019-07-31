//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#include "input.h"
#include <Arduino.h>
#include "pinout.h"

/* interface specified by rc_input.h*/
void input_setup() {
    /* input 0 is pin 3 */
    pinMode(INPUT_ADC, INPUT);
}

uint64_t input_get_current() {
    return analogRead(INPUT_ADC);
}
