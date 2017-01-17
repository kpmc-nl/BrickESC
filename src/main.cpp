#ifdef CMAKE_BUILD

#include <Arduino.h>
#include "Controller.h"

Controller controller;


int main(void) {
    // Mandatory init
    init();

    for (int i = PIN_B0; i <= PIN_B3; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);
    }
    pinMode(PIN_B1, INPUT);

    controller.setup();

    while (1) {
        controller.loop();
    }

}

#endif