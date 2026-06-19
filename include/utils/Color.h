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
     * @brief Given a background color and foreground color, compose them together using their alpha values
     */
    static Color compose(Color bg_color, Color fg_color) {
        double alpha_cf = fg_color.alpha / 255.0;
        double alpha_cb = bg_color.alpha / 255.0;

        double alpha_composite = alpha_cf + alpha_cb * (1 - alpha_cf);
        double red_composite = (fg_color.red * alpha_cf + bg_color.red * alpha_cb * (1 - alpha_cf)) / alpha_composite;
        double green_composite = (fg_color.green * alpha_cf + bg_color.green * alpha_cb * (1 - alpha_cf)) / alpha_composite;
        double blue_composite = (fg_color.blue * alpha_cf + bg_color.blue * alpha_cb * (1 - alpha_cf)) / alpha_composite;

        Color res(red_composite, green_composite, blue_composite);
        res.alpha = alpha_composite * 255;

        return res;
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