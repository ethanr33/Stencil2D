
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

    bool operator==(const Fragment& rhs) const {
        return this->x == rhs.x && this->y == rhs.y && this->color == rhs.color && this->z_index == rhs.z_index;
    }
};