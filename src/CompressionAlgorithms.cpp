
#include "image/CompressionAlgorithms.h"

uint16_t CompressionAlgorithms::get_symbol_from_fixed_huffman(uint16_t code) {
    if (code >= 0b00110000 && code <= 0b10111111) {
        return code - 0b00110000;
    } else if (code >= 0b110010000 && code <= 0b111111111) {
        return (code - 0b110010000) + 143;
    } else if (code >= 0b0000000 && code <= 0b0010111) {
        return code + 256;
    } else if (code >= 0b11000000 && code <= 0b11000111) {
        return (code - 0b11000000) + 280;
    } else {
        return 0xFFFF;
    }
}

void CompressionAlgorithms::DEFLATE_Decompress(std::vector<uint8_t>& block) {
    bool BFINAL = block.at(0) & 0b1;
    uint8_t BTYPE = (block.at(0) & 0b0110 >> 1);

    if (BTYPE == 0) {
        // Data is not compressed

        // DEFLATE uses little-endian format
        uint16_t data_length = block.at(1) | (block.at(2) << 8);
        uint16_t data_negative_length = block.at(3) | (block.at(4) << 8);

        block.erase(block.begin(), block.begin() + 5);
    } else if (BTYPE == 1) {
        // Fixed huffman block

        uint16_t stream = 0;

        for (int i = 0; i < block.size(); i++) {
            uint8_t cur_block = block.at(i);

            uint16_t litlength_code = get_symbol_from_fixed_huffman(stream & 0b01111111);

            if (litlength_code == 0xFFFF) {
                litlength_code = get_symbol_from_fixed_huffman(stream & 0b11111111);

                if (litlength_code == 0xFFFF) {
                    litlength_code = get_symbol_from_fixed_huffman(stream & 0b111111111);
                }
            }

            if (lit

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