
#include "utils/Matrix.h"
#include "utils/Exceptions.h"

Matrix::Matrix(uint32_t rows, uint32_t cols, bool identity) {
    if (rows == 0 || cols == 0) {
        throw InvalidMatrixConstructionException(rows, cols);
    }

    this->rows = rows;
    this->cols = cols;

    this->matrix = std::vector<std::vector<double>>(rows, std::vector<double>(cols));

    if (identity) {
        set_identity();
    }
}

Matrix::Matrix(const Vector& v, bool augmented=true) {
    this->rows = augmented ? 3 : 2;
    this->cols = 1;

    this->matrix = std::vector<std::vector<double>>(this->rows, std::vector<double>(this->cols));
    this->set_element(0, 0, v.x);
    this->set_element(1, 0, v.y);

    if (augmented) {
        this->set_element(2, 0, 1);
    }
}

void Matrix::set_identity() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == j) {
                this->set_element(i, j, 1);
            } else {
                this->set_element(i, j, 0);
            }
        }
    }
}

Matrix& Matrix::operator+=(const Matrix& rhs) {
    if (this->get_rows() != rhs.get_rows() || this->get_cols() != rhs.get_cols()) {
        // Matrix dimensions must match to perform addition
        throw MatrixSizeMismatchException(*this, rhs);
    }

    for (int i = 0; i < rhs.rows; i++) {
        for (int j = 0; j < rhs.cols; j++) {
            this->set_element(i, j, rhs.get_element(i, j) + this->get_element(i, j));
        }
    }

    return *this;
}

const Matrix Matrix::operator+(const Matrix& rhs) const {
    return Matrix(*this) += rhs;
}

Matrix Matrix::operator*(const Matrix& rhs) const {
    int m = this->rows;
    int n = this->cols;
    int k = rhs.cols;

    if (this->cols != rhs.rows) {
        throw MatrixSizeMismatchException(*this, rhs);
    }

    Matrix M = Matrix(m, k);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            double sum = 0;

            for (int l = 0; l < n; l++) {
                sum += this->get_element(i, l) * rhs.get_element(l, j);
            }

            M.set_element(i, j, sum);
        }
    }

    return M;
}