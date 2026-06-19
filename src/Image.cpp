
#include "image/Image.h"
#include "utils/Exceptions.h"
#include "image/CompressionAlgorithms.h"

#include <fstream>
#include <cmath>
#include <algorithm>

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

void Image::construct_fragments(uint8_t color_type, uint8_t bit_depth, const std::vector<uint8_t>& raw_data, const std::vector<Color>& palette, const std::vector<uint8_t>& transparencies, Color background_color) {
 
    int i = 0;

    uint8_t filtering_method = 0;

    uint8_t bits_per_pixel = bit_depth;
    uint8_t pixels_per_byte = 8 / bit_depth;

    struct Prefixes {
        Color left = Color(0, 0, 0);
        Color upper_left = Color(0, 0, 0);
        std::vector<Color> upper;
    } prefixes;

    prefixes.upper = std::vector<Color>(this->base_width);

    uint64_t width_index = 0;
    
    while (i < raw_data.size()) {
        if (i % ((this->base_width >> (pixels_per_byte - 1)) + 1) == 0) {
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

            for (int byte_pos = 0; byte_pos < 8; byte_pos += bit_depth) {

                uint8_t foreground_red;
                uint8_t foreground_green;
                uint8_t foreground_blue;

                uint8_t mask = 0;

                switch (pixels_per_byte) {
                    case 1:
                        mask = 0xFF;
                        break;
                    case 2:
                        mask = 0x0F;
                        break;
                    case 4:
                        mask = 0x03;
                        break;
                    case 8:
                        mask = 0x01;
                        break;
                    default:
                        break;
                }

                uint8_t shift_amt = ((8 - byte_pos) - bit_depth);

                mask = mask << shift_amt;

                uint8_t palette_index = (raw_data.at(i) & mask) >> shift_amt;

                if (color_type == 0) {
                    foreground_red = (raw_data.at(i) & mask) >> shift_amt;
                    foreground_green = (raw_data.at(i) & mask) >> shift_amt;
                    foreground_blue = (raw_data.at(i) & mask) >> shift_amt;
                } else {
                    foreground_red = palette.at(palette_index).red;
                    foreground_green = palette.at(palette_index).green;
                    foreground_blue = palette.at(palette_index).blue;
                }


                if (filtering_method == 1) {
                    if (i >= 1 + (bits_per_pixel / 8)) {
                        foreground_red += prefixes.left.red;
                        foreground_green += prefixes.left.green;
                        foreground_blue += prefixes.left.blue;
                    }
                } else if (filtering_method == 2) {
                    foreground_red += prefixes.upper.at(width_index).red;
                    foreground_green += prefixes.upper.at(width_index).green;
                    foreground_blue += prefixes.upper.at(width_index).blue;
                } else if (filtering_method == 3) {
                    // TODO: Implement average filtering method
                } else if (filtering_method == 4) {
                    // Paeth filtering

                    foreground_red += paeth_predictor(prefixes.left.red, prefixes.upper.at(width_index).red, prefixes.upper_left.red);
                    foreground_green += paeth_predictor(prefixes.left.green, prefixes.upper.at(width_index).green, prefixes.upper_left.green);
                    foreground_blue += paeth_predictor(prefixes.left.blue, prefixes.upper.at(width_index).blue, prefixes.upper_left.blue);
                }

                Color res = Color(foreground_red, foreground_green, foreground_blue);

                prefixes.left = res;

                prefixes.upper_left = prefixes.upper.at(width_index);

                prefixes.upper.at(width_index) = res;


                if (transparencies.size() > 0) {
                    res.alpha = transparencies.at(palette_index);
                    this->image_data.push_back(res);
                } else {
                    this->image_data.push_back(Color(foreground_red, foreground_green, foreground_blue));
                }

                width_index++;
           }

           i++;
           
        }
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
            } else if (filtering_method == 4) {
                // Paeth filtering

                foreground_red += paeth_predictor(prefixes.left.red, prefixes.upper.at(width_index).red, prefixes.upper_left.red);
                foreground_green += paeth_predictor(prefixes.left.green, prefixes.upper.at(width_index).green, prefixes.upper_left.green);
                foreground_blue += paeth_predictor(prefixes.left.blue, prefixes.upper.at(width_index).blue, prefixes.upper_left.blue);
                foreground_alpha += paeth_predictor(prefixes.left.alpha, prefixes.upper.at(width_index).alpha, prefixes.upper_left.alpha);
            }

            Color foreground_color = Color(foreground_red, foreground_green, foreground_blue);
            foreground_color.alpha = foreground_alpha;

            prefixes.left = foreground_color;

            prefixes.upper_left = prefixes.upper.at(width_index);

            prefixes.upper.at(width_index) = foreground_color;

            if (color_type == 4 || color_type == 6) {
                this->image_data.push_back(Color::compose(background_color, foreground_color));
            } else {
                Color res = Color(foreground_red, foreground_green, foreground_blue);
                res.alpha = 255; // Pixel is fully opaque if no alpha
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

    // For use by the tRNS chunk
    std::vector<uint8_t> transparencies;

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

            if (color_type == 2 || color_type == 4 || color_type == 6) {
                this->construct_fragments(color_type, bit_depth, decompressed, background_color);
            } else if (color_type == 0 || color_type == 3) {
                this->construct_fragments(color_type, bit_depth, decompressed, palette, transparencies, background_color);
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
                background_color.alpha = 255;
            } else if (color_type == 3) {
                // Technically this chunk should contain the palette index of the color which will be treated as transparent
                // Since we'd like to keep the background color variable consistently as a Color object though, we'll set
                // the background color to the value of the color which should be transparent

                background_color = palette.at(chunk_data[0]);
            }
        } else if (chunk_name == "tRNS") {
            // Defines transparency for colors which do not use it by default
            // This chunk is reqired to come after a PLTE chunk (if it exists)

            for (int i = 0; i < chunk_length; i++) {
                transparencies.push_back(chunk_data[i]);
            }

            if (color_type == 3) {
                // There may be less transparency entries than palette entries
                while (transparencies.size() < palette.size()) {
                    transparencies.push_back(0xFF);
                }
            }

        }

        chunk_count++;
    }
}

void Image::apply_transformation_matrix(const Matrix& transformation_matrix) {

}

void Image::rasterize(std::vector<Fragment>& fragments) {

    for (int i = 0; i < image_data.size(); i++) {
        fragments.push_back(Fragment(this->pos.x + (i % this->base_width), this->pos.y + (i / this->base_width), image_data.at(i), this->get_z_index()));
    }
}

std::unique_ptr<Renderable> Image::copy() const {
    return std::make_unique<Image>(*this);
}