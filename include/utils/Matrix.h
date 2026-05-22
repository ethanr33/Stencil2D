#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

struct Vector;

#include "utils/Vector.h"

class Matrix {
    private:
        uint32_t rows;
        uint32_t cols;

        std::vector<std::vector<double>> matrix;
    public:
        Matrix(uint32_t rows, uint32_t cols, bool identity=false);

        Matrix(const Vector&, bool);

        inline int get_rows() const noexcept {
            return rows;
        }

        inline int get_cols() const noexcept {
            return cols;
        }

        /**
         * @brief Sets element at position (row, col) to val
         */
        inline void set_element(int row, int col, double val) {
            this->matrix.at(row).at(col) = val;
        }

        /**
         * @brief Gets element at (row, col) position
         */
        inline double get_element(int row, int col) const {
            return this->matrix.at(row).at(col);
        }

        /**
         * @brief Sets the matrix to the identity matrix
         */
        void set_identity();

        Matrix& operator+=(const Matrix&);

        const Matrix operator+(const Matrix&) const;

        Matrix operator*(const Matrix&) const;

};
