//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#ifndef SC_MOTOR_H
#define SC_MOTOR_H


void motor_setup();

void motor_power(int power);

void motor_reverse();

void motor_forward();

void motor_tone(uint64_t freq, uint64_t duration);

#endif /* SC_MOTOR_H */
