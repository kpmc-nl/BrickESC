//
// Created by Matthijs Oosterhoff on 12/05/16.
//

#ifndef SPEED_CONTROLLER_UTIL_H
#define SPEED_CONTROLLER_UTIL_H


#include <stdint.h>

class Math {

public:
    static uint64_t median(uint32_t *input, uint32_t array_size);

};


#endif //SPEED_CONTROLLER_UTIL_H
