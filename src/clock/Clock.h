//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#ifndef HBRIDGE_DRIVER_CLOCK_H
#define HBRIDGE_DRIVER_CLOCK_H


#include <stdint.h>

/**
 * Defines a way to keep time using one of the built in timers (timer1) of the ATTiny.
 */
class Clock {

public:

    static void init();

    static uint64_t millis();

    static uint64_t micros();

};

#endif //HBRIDGE_DRIVER_CLOCK_H
