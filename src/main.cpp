#ifdef CMAKE_BUILD

#include <Arduino.h>
#include "Controller.h"

Controller controller;

int main(void) {
    // Mandatory init
    init();

    controller.setup();

    while (true) {
        controller.loop();
    }

}

#endif