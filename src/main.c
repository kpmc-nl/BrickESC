
#include <Arduino.h>
#include <avr/power.h>
#include "input.h"
#include "motor.h"
#include "controller.h"


int main(void) {
    /* Mandatory init for AttinyCore library */
    init();

    /* Run at 8MHz */
    clock_prescale_set(clock_div_1);

    /* Use the 5v on AREF as analog reference */
    analogReference(EXTERNAL);

    /* setup components */
    input_setup();
    motor_setup();
    controller_setup();


    /* The main logic loop */
    while (true) {
        controller_loop();
    }

}