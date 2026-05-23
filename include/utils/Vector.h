
#pragma once

class Matrix;

#include "utils/Matrix.h"

/**
 * @brief Represents a vector in 2D space in column vector format
 */
struct Vector {
    double x;
    double y;

    Vector() : x(0), y(0) {}
    Vector(double x, double y) : x(x), y(y) {}

    /**
     * @brief constructs a vector from a m x 1 matrix
     * Uses the first row as x, second row as y. All other rows are ignored.
     */
    Vector(const Matrix&);

};
