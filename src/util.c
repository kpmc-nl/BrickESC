//
// Created by Matthijs Oosterhoff on 25/05/16.
//

#include "util.h"

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


uint64_t median(uint64_t *input, uint64_t array_size) {

    uint64_t sorted[array_size];
    for (int i = 0; i < array_size; ++i) {
        sorted[i] = input[i];
    }
    for (int i = array_size - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (sorted[j] > sorted[j + 1]) {
                uint64_t tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    // Middle or average of middle values in the sorted input.
    uint64_t res = 0;
    if ((array_size % 2) == 0) {
        res = ((sorted[array_size / 2] / 2) + (sorted[(array_size / 2) - 1]) / 2);
    } else {
        res = sorted[array_size / 2];
    }
    return res;

}