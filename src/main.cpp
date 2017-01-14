#ifdef CMAKE_BUILD

#include <Arduino.h>
#include <avr/power.h>
#include "Controller.h"
#include "main.h"


Settings settings;
Controller controller;

int main(void) {
    // Mandatory init
    init();



    FUSE_BODACT0

    // run at 8mhz
    clock_prescale_set(clock_div_1);

    analogReference(EXTERNAL);

    controller.setup();

    while (1) {
        controller.loop();
    }

}

#endif