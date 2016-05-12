//
// Created by Matthijs Oosterhoff on 12/05/16.
//

#include "Math.h"


uint64_t Math::median(uint32_t *input, uint32_t array_size) {

    uint32_t sorted[array_size];
    for (int i = 0; i < array_size; ++i) {
        sorted[i] = input[i];
    }
    for (int i = array_size - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (sorted[j] > sorted[j + 1]) {
                uint32_t tmp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = tmp;
            }
        }
    }

    // Middle or average of middle values in the sorted input.
    uint32_t res = 0;
    if ((array_size % 2) == 0) {
        res = ((sorted[array_size / 2] / 2) + (sorted[(array_size / 2) - 1]) / 2);
    } else {
        res = sorted[array_size / 2];
    }
    return res;

}