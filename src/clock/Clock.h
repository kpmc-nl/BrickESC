//
// Created by Matthijs Oosterhoff on 28/02/16.
//

#ifndef HBRIDGE_DRIVER_CLOCK_H
#define HBRIDGE_DRIVER_CLOCK_H

/* Clock class should be enabled explicitly as it claims a timer which might be needed for other purposes. */
#ifdef ENABLE_CLOCK

#include <stdint.h>

class Clock {

public:

    static void init();

    static uint64_t millis();

    static uint64_t micros();

};

#endif

#endif //HBRIDGE_DRIVER_CLOCK_H
