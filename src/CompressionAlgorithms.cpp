
#include "image/CompressionAlgorithms.h"

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
    } else if (BTYPE == 2) {
        uint8_t HLIT = (block.at(0) & 0x11111000) >> 3;
        uint8_t HDIST = block.at(1) & 0x00011111;
        uint8_t HCLEN = ((block.at(1) & 0x11100000) >> 5) | ((block.at(2) & 0x1) << 4);
    } else {
        // BTYPE should not be 3 in a valid PNG file
        return;
    }
}