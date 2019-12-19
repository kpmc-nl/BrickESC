//
// Created by Matthijs Oosterhoff on 17/01/2017.
//

#ifndef SC_RC_H
#define SC_RC_H

#define RC_PWM_MIN 1000
#define RC_PWM_MAX 2000
#define RC_PWM_NEUTRAL 1500
#define RC_PWM_OUTER_THRESH 20
#define RC_PWM_INNER_THRESH 50

#define RC_PWM_MINIMUM_FORWARD_SPEED  0    // min 0 .. max 255 (255 is switc
#define RC_PWM_MOTOR_FORWARD_SPEED_MAX 255 // max 255
#define RC_PWM_MINIMUM_REVERSE_SPEED  0    // min 0 .. max 255 (255 is switc
#define RC_PWM_MOTOR_REVERSE_SPEED_MAX 255 // max 255

#define RC_PWM_LOOPTIME_CONTROLLER_DELAY_RUNNING 1400

#endif /* SC_RC_H */
