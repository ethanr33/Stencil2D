
#include <deque>

#include "image/CompressionAlgorithms.h"
#include "utils/Bitstream.h"

uint16_t CompressionAlgorithms::get_litlength_from_fixed_huffman(uint16_t code, uint8_t code_length) {
        if (code >= 0b0000000 && code <= 0b0010111 && code_length == 7) {
            return code + 256;
        } else if (code >= 0b00110000 && code <= 0b10111111 && code_length == 8) {
            return code - 0b00110000;
        } else if (code >= 0b11000000 && code <= 0b11000111 && code_length == 8) {
            return (code - 0b11000000) + 280;
        } else if (code >= 0b110010000 && code <= 0b111111111 && code_length == 9) {
            return (code - 0b110010000) + 144;
        } else {
            return 0xFFFF;
        }
}

uint8_t CompressionAlgorithms::get_bits_for_length_code(uint16_t code) {
    if ((code >= 257 && code <= 264) || code == 285) {
        return 0;
    } else if (code >= 265 && code <= 268) {
        return 1;
    } else if (code >= 270 && code <= 272) {
        return 2;
    } else if (code >= 273 && code <= 276) {
        return 3;
    } else if (code >= 277 && code <= 280) {
        return 4;
    } else if (code >= 281 && code <= 284) {
        return 5;
    } else {
        // invalid
        return 0xFF;
    }
}

void CompressionAlgorithms::DEFLATE_Decompress(std::vector<uint8_t>& block, std::vector<uint8_t>& decompressed) {

    Bitstream bitstream;

    // Initialize bitstream
    for (int i = 0; i < block.size(); i++) {
        bitstream.push_byte(block.at(i));
    }

    bool BFINAL = bitstream.pop() & 0b1;
    uint8_t BTYPE = bitstream.pop_num(2);

    if (BTYPE == 0) {
        // Data is not compressed

        // DEFLATE uses little-endian format
        uint16_t data_length = block.at(1) | (block.at(2) << 8);
        uint16_t data_negative_length = block.at(3) | (block.at(4) << 8);

        block.erase(block.begin(), block.begin() + 5);
    } else if (BTYPE == 1) {
        // Fixed huffman block



        while (!bitstream.is_empty()) {
            uint16_t code = bitstream.pop_num(7, true);

            // Get first 7 bits bc code is 7-9 bits long

            uint16_t litlength_code = get_litlength_from_fixed_huffman(code, 7);

            if (litlength_code == 0xFFFF) {
                code = (code << 1) | bitstream.pop();

                litlength_code = get_litlength_from_fixed_huffman(code, 8);
                
                if (litlength_code == 0xFFFF) {
                    code = (code << 1) | bitstream.pop();
                    litlength_code = get_litlength_from_fixed_huffman(code, 9);
                }
            }

            if (litlength_code <= 0xFF) {
                // Value is a literal byte
                decompressed.push_back(litlength_code);
            } else if (litlength_code > 0x100) {
                // Value is the length in a (length, distance) pair
                uint8_t num_bits = get_bits_for_length_code(litlength_code);
                uint16_t total_length = FIXED_HUFFMAN_LENGTH_BASE.at(litlength_code);
                uint8_t length_extra = bitstream.pop_num(num_bits);

                total_length += length_extra;

                // Find distance

                uint8_t dist_code = bitstream.pop_num(5);

                // Distance code is always 5 bits

                uint16_t total_dist = FIXED_HUFFMAN_DIST_BASE.at(dist_code);
                uint8_t dist_offset_bits = FIXED_HUFFMAN_DIST_NUM_BITS.at(dist_code);
                
                if (dist_offset_bits == 0) {
                    total_dist = (dist_code + 1);
                } else {
                    uint16_t dist_offset = bitstream.pop_num(dist_offset_bits);

                    total_dist += dist_offset;
                }

                // Add bytes to stream from (length, distance) pair

                size_t start_pos = decompressed.size();

                for (int i = 0; i < total_length; i++) {
                    size_t byte_pos = start_pos - total_dist + i;
                    decompressed.push_back(decompressed.at(byte_pos));
                }
            } else {
                // 256 is the EOB symbol
                break;
            }
        }

    } else if (BTYPE == 2) {
        uint8_t HLIT = (block.at(0) & 0x11111000) >> 3;
        uint8_t HDIST = block.at(1) & 0x00011111;
        uint8_t HCLEN = ((block.at(1) & 0x11100000) >> 5) | ((block.at(2) & 0x1) << 4);

        // Read data/lengths from huffman tree
        uint16_t num_lit_length_codes = HLIT + 257;
    } else {
        // BTYPE should not be 3 in a valid PNG file
        return;
    }
}