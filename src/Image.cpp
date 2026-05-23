
#include "image/Image.h"
#include "utils/Exceptions.h"

#include <fstream>
#include <array>

Image::Image(const std::string& file_path) {
    Image::FILE_FORMAT file_type = this->determine_file_format(file_path);

    switch (file_type) {
        case Image::FILE_FORMAT::PNG:
            this->load_PNG(file_path);
            break;
        default:
            throw UnknownImageFormatException(file_path);
            break;
    }
}

Image::FILE_FORMAT Image::determine_file_format(const std::string& file_path) const {

    std::ifstream input_file(file_path, std::ios::binary);

    std::array<uint8_t, 8> header;

    for (int i = 0; i < header.size(); i++) {
        char* byte;

        input_file.read(byte, 1);
        header.at(i) = (uint8_t) *byte;
    }

    input_file.close();

}

void Image::load_PNG(const std::string& file_path) {

}