#ifdef CMAKE_BUILD

#include <Arduino.h>
#include "Controller.h"

Controller controller;


int main(void) {
    // Mandatory init
    init();

    controller.setup();

    while (1) {
        controller.loop();
    }

}

#endif