
#include "image/Image.h"
#include "utils/Exceptions.h"
#include "image/CompressionAlgorithms.h"

#include <fstream>
#include <cmath>

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

uint8_t paeth_predictor(uint8_t a, uint8_t b, uint8_t c) {
    int p = a + b - c;
    int pa = std::abs(p - a);
    int pb = std::abs(p - b);
    int pc = std::abs(p - c);

    if (pa <= pb && pa <= pc) {
        return a;
    } else if (pb <= pc) {
        return b;
    } else {
        return c;
    }
}

void Image::construct_fragments(uint8_t color_type, uint8_t bit_depth, const std::vector<uint8_t>& raw_data, Color background_color) {
    
    int i = 0;

    uint8_t filtering_method = 0;

    uint8_t bits_per_pixel;

    switch (color_type) {
        case 2:
            // True color
            bits_per_pixel = 3 * bit_depth;
            break;
        case 4:
            // Grayscale and alpha
            bits_per_pixel = 2 * bit_depth;
            break;
        case 6:
            // True color and alpha
            bits_per_pixel = 4 * bit_depth;
            break;
        default:
            // Invalid color type
            return;
    }

    struct Prefixes {
        Color left = Color(0, 0, 0);
        Color upper_left = Color(0, 0, 0);
        std::vector<Color> upper;
    } prefixes;

    prefixes.upper = std::vector<Color>(this->base_width);

    uint64_t width_index;
    
    while (i < raw_data.size()) {
        if (i % (this->base_width * (bits_per_pixel / 8) + 1) == 0) {
            // Determine filtering method
            filtering_method = raw_data.at(i);
            // Update prefixes for filtering
            prefixes.left = Color(0, 0, 0);
            prefixes.upper_left = Color(0, 0, 0);
            width_index = 0;
            i++;
        } else {
            // Read and process pixel

            // Calculate pixel color taking into account alpha
            // Let "cf" be the color of the foreground pixel, "cb" is the background color

            uint8_t foreground_red;
            uint8_t foreground_green;
            uint8_t foreground_blue;
            uint8_t foreground_alpha;

            if (color_type == 2) {
                // Alpha is not supported here
                if (bit_depth == 8) {
                    foreground_red = raw_data.at(i + 1);
                    foreground_green = raw_data.at(i + 2);
                    foreground_blue = raw_data.at(i + 3);
                } else if (bit_depth == 16) {
                    foreground_red = (raw_data.at(i + 1) << 8) | raw_data.at(i + 2);
                    foreground_green = (raw_data.at(i + 3) << 8) | raw_data.at(i + 4);
                    foreground_blue = (raw_data.at(i + 5) << 8) | raw_data.at(i + 6);
                }
            } else if (color_type == 6) {
                // Alpha is suppprted here
                if (bit_depth == 8) {
                    foreground_red = raw_data.at(i);
                    foreground_green = raw_data.at(i + 1);
                    foreground_blue = raw_data.at(i + 2);
                    foreground_alpha = raw_data.at(i + 3);
                } else if (bit_depth == 16) {
                    foreground_red = (raw_data.at(i) << 8) | raw_data.at(i + 1);
                    foreground_green = (raw_data.at(i + 2) << 8) | raw_data.at(i + 3);
                    foreground_blue = (raw_data.at(i + 4) << 8) | raw_data.at(i + 5);
                    foreground_alpha = (raw_data.at(i + 6) << 8) | raw_data.at(i + 7);
                }
            } else {
                // Grayscale with alpha
                if (bit_depth == 8) {
                    foreground_red = raw_data.at(i);
                    foreground_green = raw_data.at(i);
                    foreground_blue = raw_data.at(i);
                    foreground_alpha = raw_data.at(i + 1);
                } else if (bit_depth == 16) {
                    foreground_red = (raw_data.at(i) << 8) | raw_data.at(i + 1);
                    foreground_green = (raw_data.at(i) << 8) | raw_data.at(i + 1);
                    foreground_blue = (raw_data.at(i) << 8) | raw_data.at(i + 1);
                    foreground_alpha = (raw_data.at(i + 2) << 8) | raw_data.at(i + 3);
                }
            }

            if (filtering_method == 1) {
                if (i >= 1 + (bits_per_pixel / 8)) {
                    foreground_red += prefixes.left.red;
                    foreground_green += prefixes.left.green;
                    foreground_blue += prefixes.left.blue;
                    foreground_alpha += prefixes.left.alpha;
                }
            } else if (filtering_method == 2) {
                foreground_red += prefixes.upper.at(width_index).red;
                foreground_green += prefixes.upper.at(width_index).green;
                foreground_blue += prefixes.upper.at(width_index).blue;
                foreground_alpha += prefixes.upper.at(width_index).alpha;
            } else if (filtering_method == 3) {
                // TODO: Implement average filtering method
            } else {
                // Paeth filtering

                foreground_red += paeth_predictor(prefixes.left.red, prefixes.upper.at(width_index).red, prefixes.upper_left.red);
                foreground_green += paeth_predictor(prefixes.left.green, prefixes.upper.at(width_index).green, prefixes.upper_left.green);
                foreground_blue += paeth_predictor(prefixes.left.blue, prefixes.upper.at(width_index).blue, prefixes.upper_left.blue);
                foreground_alpha += paeth_predictor(prefixes.left.alpha, prefixes.upper.at(width_index).alpha, prefixes.upper_left.alpha);
            }

            prefixes.left = Color(foreground_red, foreground_green, foreground_blue);
            prefixes.left.alpha = foreground_alpha;

            prefixes.upper_left = prefixes.upper.at(width_index);

            prefixes.upper.at(width_index) = Color(foreground_red, foreground_green, foreground_blue);
            prefixes.upper.at(width_index).alpha = foreground_alpha;

            if (color_type == 4 || color_type == 6) {
                double alpha_cf = foreground_alpha / 255.0;
                double alpha_cb = background_color.alpha / 255.0;

                double alpha_composite = alpha_cf + alpha_cb * (1 - alpha_cf);
                double red_composite = (foreground_red * alpha_cf + background_color.red * alpha_cb * (1 - alpha_cf)) / alpha_composite;
                double green_composite = (foreground_green * alpha_cf + background_color.green * alpha_cb * (1 - alpha_cf)) / alpha_composite;
                double blue_composite = (foreground_blue * alpha_cf + background_color.blue * alpha_cb * (1 - alpha_cf)) / alpha_composite;

                Color res(red_composite, green_composite, blue_composite);
                res.alpha = alpha_composite;

                this->image_data.push_back(res);
            } else {
                Color res = Color(foreground_red, foreground_green, foreground_blue);
                res.alpha = 1; // Pixel is fully opaque if no alpha
                this->image_data.push_back(Color(foreground_red, foreground_green, foreground_blue));
            }

            width_index++;

            // Increment to next pixel
            i += std::ceil(bits_per_pixel / 8);
        }
    }
}

// TODO: CRC validation
void Image::load_PNG(const std::string& file_path) {

    std::ifstream input_file(file_path, std::ios::binary);

    if (!input_file) {
        throw FileNotFoundException(file_path);
    }

    // Keep track of current pixel row/col to fill in image data 
    uint32_t cur_row = 0;
    uint32_t cur_col = 0;

    uint8_t bit_depth; // Number of bits per sample
    uint8_t color_type;
    uint8_t compression_method;
    uint8_t filter_method;
    uint8_t interlace_method;

    // Palette index For use in color type 3 and palette chunk
    std::vector<Color> palette;

    // Background color when pixels are transparent
    Color background_color;

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

        uint8_t chunk_data[chunk_length];

        input_file.read((char*) chunk_data, chunk_length);

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
                this->base_width = (chunk_data[0] << 24) | (chunk_data[1] << 16) | (chunk_data[2] << 8) | chunk_data[3];
                this->base_height = (chunk_data[4] << 24) | (chunk_data[5] << 16) | (chunk_data[6] << 8) | chunk_data[7];

                bit_depth = chunk_data[8];
                color_type = chunk_data[9];
                compression_method = chunk_data[10];
                filter_method = chunk_data[11];
                interlace_method = chunk_data[12];
            }
        } else if (chunk_name == "IDAT") {
            // Streams are stored in the zlib format
            // https://www.libpng.org/pub/png/spec/1.2/PNG-Compression.html
            // https://www.ietf.org/rfc/rfc1950.txt

            uint8_t zlib_compression_method = chunk_data[0] & 0b1111;
            uint8_t zlib_compression_info = (chunk_data[0] & 0b11110000) >> 4;
            uint8_t zlib_flags = chunk_data[1];

            std::vector<uint8_t> compressed_data(chunk_length - 5);

            for (int i = 2; i < chunk_length - 4; i++) {
                compressed_data.at(i - 2) = (uint8_t) chunk_data[i];
            }

            std::vector<uint8_t> decompressed;

            CompressionAlgorithms::DEFLATE_Decompress(compressed_data, decompressed);

            // Add decompressed image data to image object

            // TODO: Handle different bit depths/color type combinations

            if (color_type == 2) {
                // Represents standard RGB format
                this->construct_fragments(2, bit_depth, decompressed, background_color);
            } else if (color_type == 6) {
                this->construct_fragments(6, bit_depth, decompressed, background_color);
            } else if (color_type == 3) {
                for (int i = 0; i < decompressed.size(); i++) {
                    // Skip over first byte because it determines filtering method
                    // TODO: Implement filtering methods

                    if (bit_depth == 4) {
                        // This is filtering mode 0
                        if (i % ((this->base_width >> 1) + 1) != 0) {
                            this->image_data.push_back(palette.at((decompressed.at(i) & 0xF0) >> 4));
                            this->image_data.push_back(palette.at((decompressed.at(i) & 0x0F)));
                        }
                    } else {
                        if (i % (this->base_width + 1) != 0) {
                            this->image_data.push_back(palette.at(decompressed.at(i)));
                        }
                    }
                }

            }


            // TODO: Checksum and header validation for DEFLATE block
        } else if (chunk_name == "PLTE") {
            // Contains 1-256 palette entries
            for (int i = 0; i < chunk_length; i += 3) {
                // Bit depth in palette is 4
                palette.push_back(Color(chunk_data[i], chunk_data[i + 1], chunk_data[i + 2]));
            }
        } else if (chunk_name == "bKGD") {
            // Chunk to set default background color
            if (color_type == 6) {
                background_color = Color(chunk_data[1], chunk_data[3], chunk_data[5]);

            }
        }

        chunk_count++;
    }
}

void Image::apply_transformation_matrix(const Matrix& transformation_matrix) {

}

void Image::rasterize(std::vector<Fragment>& fragments) {
    for (int i = 0; i < image_data.size(); i++) {
        if (i / this->base_width == 500) {
            std::cout << "got here" << std::endl;   
        }
        fragments.push_back(Fragment(this->pos.x + (i % this->base_width), this->pos.y + (i / this->base_width), image_data.at(i), this->get_z_index()));
    }
}

std::unique_ptr<Renderable> Image::copy() const {
    return std::make_unique<Image>(*this);
}