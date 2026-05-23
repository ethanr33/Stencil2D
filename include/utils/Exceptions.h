
#include <stdexcept>

#include "utils/Matrix.h"
#include "image/Image.h"

class InvalidMatrixConstructionException : public std::logic_error {
    public:
        InvalidMatrixConstructionException(uint32_t rows, uint32_t cols) : std::logic_error(
            "Cannot construct matrix of size " + std::to_string(rows) + "x" + std::to_string(cols)
        ) {}
};

class MatrixSizeMismatchException : public std::logic_error {
    public:
        MatrixSizeMismatchException(const Matrix& a, const Matrix& b) : std::logic_error(
            "Operation cannot be performed for matrices of size " + std::to_string(a.get_rows()) + "x" + std::to_string(a.get_cols()) + " and size " + std::to_string(b.get_rows()) + "x" + std::to_string(b.get_cols())
        ) {}
};

class MatrixToVectorConversionException : public std::logic_error {
    public:
        MatrixToVectorConversionException(const Matrix& m) : std::logic_error(
            "Cannot convert " + std::to_string(m.get_rows()) + "x" + std::to_string(m.get_cols()) + " matrix to column vector"
        ) {}
};

class FileNotFoundException : public std::runtime_error {
    public:
        FileNotFoundException(const std::string& file_path) : std::runtime_error(
            "Cannot find file at " + file_path
        ) {}
}

class UnknownImageFormatException : public std::runtime_error {
    public:
        UnknownImageFormatException(const std::string& file_path) : std::runtime_error(
            "Unknown or unsupported file format for image file " + file_path
        ) {}
};