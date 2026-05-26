
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

    if (!input_file) {
        throw FileNotFoundException(file_path);
    }

    // PNG header size is 8 bytes
    const int MAX_HEADER_SIZE = 8;

    char header[MAX_HEADER_SIZE + 1];
    header[MAX_HEADER_SIZE] = '\0';
    input_file.read(header, MAX_HEADER_SIZE);

    std::string header_str(header);

    input_file.close();

    if (header_str == "\211\120\116\107\15\12\32\12") {
        return Image::FILE_FORMAT::PNG;
    } else {
        return Image::FILE_FORMAT::UNKNOWN;
    }


}

void Image::load_PNG(const std::string& file_path) {

    std::ifstream input_file(file_path, std::ios::binary);

    if (!input_file) {
        throw FileNotFoundException(file_path);
    }

    // Keep track of current pixel row/col to fill in image data 
    uint32_t cur_row = 0;
    uint32_t cur_col = 0;

    uint8_t bit_depth;
    uint8_t color_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;

    // Skip PNG file header
    input_file.seekg(8);

    uint32_t chunk_length = 0;
    std::string chunk_name = "";

    uint32_t chunk_count = 0;

    while (chunk_name != "IEND") {

        chunk_length = 0;
        chunk_name = "";

        // Read in data for current chunk

        char length_data[4];
        input_file.read(length_data, 4);

        chunk_length = ((uint8_t) length_data[0] << 24) | ((uint8_t) length_data[1] << 16) | ((uint8_t) length_data[2] << 8) | (uint8_t) length_data[3];

        char name_data[5];
        // Make sure name string is null terminated so it can properly be converted into a std::string
        name_data[4] = '\0';

        input_file.read(name_data, 4);

        chunk_name = name_data;

        char chunk_data[chunk_length];

        input_file.read(chunk_data, chunk_length);

        char CRC[4];
        input_file.read(CRC, 4);

        // From PNG file format specifications, first chunk should always be IHDR
        if (chunk_count == 0 && chunk_name != "IHDR") {
            throw UnexpectedFileFormatException(file_path);
        }

        if (chunk_name == "IHDR") {
             if (chunk_count > 0) {
                // Header chunk can only appear as the first chunk
                throw UnexpectedFileFormatException(file_path);
            } else {
                this->base_width = ((uint8_t) chunk_data[0] << 24) | ((uint8_t) chunk_data[1] << 16) | ((uint8_t) chunk_data[2] << 8) | (uint8_t) chunk_data[3];
                this->base_height = ((uint8_t) chunk_data[4] << 24) | ((uint8_t) chunk_data[5] << 16) | ((uint8_t) chunk_data[6] << 8) | (uint8_t) chunk_data[7];

                bit_depth = (unsigned char) chunk_data[8];
                color_type = (unsigned char) chunk_data[9];
                compression_method = (unsigned char) chunk_data[10];
                filter_method = (unsigned char) chunk_data[11];
                interlace_method = (unsigned char) chunk_data[12];

                image_data = std::vector<std::vector<Color>>(this->base_height, std::vector<Color>(this->base_width));
            }
        } else if (chunk_name == "IDAT") {
            for (int i = 0; i < chunk_length; i++) {
                
            }
        }

        chunk_count++;
    }
}

void Image::apply_transformation_matrix(const Matrix& transformation_matrix) {

}

void Image::rasterize(std::vector<Fragment>& fragments) {

}

std::unique_ptr<Renderable> Image::copy() const {
    return std::make_unique<Image>(*this);
}