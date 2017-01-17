//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#ifndef SC_UTIL_H
#define SC_UTIL_H


#include <avr/io.h>

long map(long x, long in_min, long in_max, long out_min, long out_max);

uint64_t median(uint64_t *input, uint64_t array_size);

#endif /* SC_UTIL_H */
