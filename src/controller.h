//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#ifndef SC_CONTROLLER_H
#define SC_CONTROLLER_H

void wait_for_neutral();

int get_battery_voltage();

void controller_setup();

void controller_loop();


#endif /* SC_CONTROLLER_H */
