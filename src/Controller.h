//
// Created by Matthijs Oosterhoff on 26/05/16.
//

#ifndef SC_CONTROLLER_H
#define SC_CONTROLLER_H

#define LED1_PIN PIN_A6
#define LED2_PIN PIN_A5
#define FET_PIN PIN_A7
#define RELAY_PIN PIN_B2


#include "Settings.h"
#include "RCInput.h"

class Controller {

private:
    Settings settings;
    uint64_t target_pulse;

public:
    Controller();

    void setup();

    void loop();

};


#endif //SC_CONTROLLER_H
