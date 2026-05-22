
#pragma once

#include "utils/Color.h"

/**
 * @brief Represents information on how to shade a single pixel
 */
struct Fragment {
    int32_t x;
    int32_t y;
    Color color;
    uint32_t z_index;

    Fragment(int32_t x, int32_t y, Color color, uint32_t z_index) : x(x), y(y), color(color), z_index(z_index) {}
};