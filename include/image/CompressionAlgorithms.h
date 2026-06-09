
#pragma once

#include <vector>
#include <cstdint>
#include <unordered_map>

/**
 * @brief Utility class defining image decompression algorithms
 * 
 */
class CompressionAlgorithms {
    private:
        /**
         * Given a code (or partial code) of length 7-9 bits, return:
         * -1 if the code does not match any symbol
         * An integer corresponding to the symbol in the lit/length alphabet
         */
        static uint16_t get_litlength_from_fixed_huffman(uint16_t code, uint8_t code_length);

        static std::vector<uint16_t> get_huffman_mapping(const std::vector<uint8_t>& code_lengths);

        inline static const std::unordered_map<uint16_t, uint16_t> FIXED_HUFFMAN_LENGTH_BASE = {
            {257, 3},
            {258, 4},
            {259, 5},
            {260, 6},
            {261, 7},
            {262, 8},
            {263, 9},
            {264, 10},
            {265, 11},
            {266, 13},
            {267, 15},
            {268, 17},
            {269, 19},
            {270, 23},
            {271, 27},
            {272, 31},
            {273, 35},
            {274, 43},
            {275, 51},
            {276, 59},
            {277, 67},
            {278, 83},
            {279, 99},
            {280, 115},
            {281, 131},
            {282, 163},
            {283, 195},
            {284, 227},
            {285, 258},
        };

        inline static const std::unordered_map<uint8_t, uint16_t> FIXED_HUFFMAN_DIST_BASE = {
            {0,  1},
            {1,  2},
            {2,  3},
            {3,  4},
            {4,  5},
            {5,  7},
            {6,  9},
            {7,  13},
            {8,  17},
            {9,  25},
            {10, 33},
            {11, 49},
            {12, 65},
            {13, 97},
            {14, 129},
            {15, 193},
            {16, 257},
            {17, 385},
            {18, 513},
            {19, 769},
            {20, 1025},
            {21, 1537},
            {22, 2049},
            {23, 3073},
            {24, 4097},
            {25, 6145},
            {26, 8193},
            {27, 12289},
            {28, 16385},
            {29, 24577},
        };

        inline static const std::unordered_map<uint8_t, uint8_t> FIXED_HUFFMAN_DIST_NUM_BITS = {
            {0,  0},
            {1,  0},
            {2,  0},
            {3,  0},
            {4,  1},
            {5,  1},
            {6,  2},
            {7,  2},
            {8,  3},
            {9,  3},
            {10, 4},
            {11, 4},
            {12, 5},
            {13, 5},
            {14, 6},
            {15, 6},
            {16, 7},
            {17, 7},
            {18, 8},
            {19, 8},
            {20, 9},
            {21, 9},
            {22, 10},
            {23, 10},
            {24, 11},
            {25, 11},
            {26, 12},
            {27, 12},
            {28, 13},
            {29, 13},
        };

        /**
         * @brief Get the number of bits that need to be read for a given length code
         * 
         * @param code 
         * @return uint8_t 
         */
        static uint8_t get_bits_for_length_code(uint16_t code);

    public:
        /**
         * @brief Decompress a block compressed using DEFLATE
         */
        static void DEFLATE_Decompress(std::vector<uint8_t>& block, std::vector<uint8_t>& decompressed);
        static void LZ77_Decode();
};