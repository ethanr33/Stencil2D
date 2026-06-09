#pragma once

#include <cstdint>

/**
 * @class Represents a color in RGB format
 */
struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

    Color() : red(0), green(0), blue(0), alpha(0) {}
    Color(uint16_t red, uint16_t green, uint16_t blue, uint8_t bit_depth = 8) : alpha(0xFF) {
        if (bit_depth == 8) {
            this->red = red;
            this->green = green;
            this->blue = blue;
        } else if (bit_depth == 4) {
            this->red = ((double) red / 255) * 16;
            this->green = ((double) green / 255) * 16;
            this->blue = ((double) blue / 255) * 16;
        }
    }

    /**
     * @brief Converts object into ARGB32 format
     */
    uint32_t to_hex() const {
        return (alpha << 24) | (red << 16) | (green << 8) | blue;
    }

    bool operator==(const Color& rhs) const {
        return this->to_hex() == rhs.to_hex();
    }

    bool operator!=(const Color& rhs) const {
        return !(*this == rhs);
    }

};