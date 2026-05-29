
#include <vector>
#include <cstdint>

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
        static uint16_t get_symbol_from_fixed_huffman(uint16_t code);
    public:
        /**
         * @brief Decompress a block compressed using DEFLATE
         */
        static void DEFLATE_Decompress(std::vector<uint8_t>& block);
        static void LZ77_Decode();
};