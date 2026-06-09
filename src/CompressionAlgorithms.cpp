
#include <deque>
#include <algorithm>

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
    } else if (code >= 269 && code <= 272) {
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

std::vector<uint16_t> CompressionAlgorithms::get_huffman_mapping(const std::vector<uint8_t>& code_lengths) {
    std::vector<uint32_t> code_length_count(*std::max_element(code_lengths.begin(), code_lengths.end()) + 1, 0);

    for (const uint8_t length : code_lengths) {
        if (length != 0) {
            code_length_count.at(length)++;
        }
    }

    // Find numerical value of smallest code for each code length

    std::vector<uint16_t> min_code_for_length(code_length_count.size(), 0);

    uint16_t code = 0;
    for (int bits = 1; bits < code_length_count.size(); bits++) {
        code = (code + code_length_count.at(bits - 1)) << 1;
        min_code_for_length.at(bits) = code;
    }

    // Assign numerical values to each code

    std::vector<uint16_t> codes(code_lengths.size(), 0);

    for (int i = 0; i < codes.size(); i++) {
        uint8_t len = code_lengths.at(i);
        if (len > 0) {
            codes.at(i) = min_code_for_length.at(len);
            min_code_for_length.at(len)++;
        } else {
            codes.at(i) = -1;
        }
    }

    return codes;

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
        uint8_t HLIT = bitstream.pop_num(5);
        uint8_t HDIST = bitstream.pop_num(5);
        uint8_t HCLEN = bitstream.pop_num(4);

        uint16_t num_lit_length_codes = HLIT + 257;
        uint16_t num_dist_codes = HDIST + 1;
        uint16_t num_code_length_codes = HCLEN + 4;

        // Read data/lengths from huffman tree

        std::vector<uint8_t> code_length_alphabet_order = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

        // Get length of code length codes
        std::vector<uint8_t> code_lengths(code_length_alphabet_order.size(), 0);

        for (int i = 0; i < num_code_length_codes; i++) {
            code_lengths.at(code_length_alphabet_order.at(i)) = bitstream.pop_num(3);
        }

        std::vector<uint16_t> length_codes = get_huffman_mapping(code_lengths);

        // Map from a binary huffman code to a (symbol, length) pair
        // If a code does not exist, then it is not a valid huffman code
        // If it does exist, then we need to check if the length of the code matches
        std::unordered_map<uint32_t, uint8_t> code_to_symbol_cl;

        for (int i = 0; i < length_codes.size(); i++) {
            if (length_codes.at(i) != 0xFFFF) {
                code_to_symbol_cl.insert({(length_codes.at(i) << 16) | code_lengths.at(i), i});
            }
        }

        // Next, decode data/length huffman tree

        // literal/length and distance code encodings overlap each other
        std::vector<uint8_t> huffman_code_lengths;

        // Code length codes can cross over between literal/lengths and distances
        while (huffman_code_lengths.size() < num_lit_length_codes + num_dist_codes) {
            uint8_t cur_code_length_code = 0;
            int num_bits_popped = 0;


            // Check if code exists, and if the code length calculated matches the current code's length
            while (code_to_symbol_cl.find((cur_code_length_code << 16) | num_bits_popped) == code_to_symbol_cl.end()) {
                cur_code_length_code = (cur_code_length_code << 1) | bitstream.pop();
                num_bits_popped++;
            }

            // Find symbol of found huffman code

            uint16_t cur_symbol = code_to_symbol_cl.at((cur_code_length_code << 16) | num_bits_popped);

            // Decode symbol and see what to do

            if (cur_symbol <= 15) {
                // Literal code length value
                huffman_code_lengths.push_back(cur_symbol);
            } else if (cur_symbol == 16) {
                // Copy previous code length 3-6 times
                uint8_t num_repeats = bitstream.pop_num(2) + 3;

                uint8_t prev_code_length = huffman_code_lengths.at(huffman_code_lengths.size() - 1);

                for (int j = 0; j < num_repeats; j++) {
                    huffman_code_lengths.push_back(prev_code_length);
                }
            } else if (cur_symbol == 17) {
                // Repeat a code length of 0 for 3-10 times
                uint8_t num_repeats = bitstream.pop_num(3) + 3;

                for (int j = 0; j < num_repeats; j++) {
                    huffman_code_lengths.push_back(0);
                }
            } else if (cur_symbol == 18) {
                // Repeat a code length of 0 for 11 - 138 times
                uint8_t num_repeats = bitstream.pop_num(7) + 11;

                for (int j = 0; j < num_repeats; j++) {
                    huffman_code_lengths.push_back(0);
                }
            }

            
        }

        // Next, find encodings for literal/length and distance huffman trees

        std::vector<uint8_t> litlength_code_lengths(huffman_code_lengths.begin(), huffman_code_lengths.begin() + num_lit_length_codes);

        std::vector<uint8_t> distance_code_lengths(huffman_code_lengths.begin() + num_lit_length_codes, huffman_code_lengths.begin() + num_lit_length_codes + num_dist_codes);

        std::vector<uint16_t> litlength_codes = get_huffman_mapping(litlength_code_lengths);
        std::vector<uint16_t> distance_codes = get_huffman_mapping(distance_code_lengths);

        // Maps potential huffman encoding of literal/length to (symbol, code length) pair
        std::unordered_map<uint32_t, uint16_t> code_to_litlength_symbol;
        // Maps potential huffman encoding of distance to (symbol, code length) pair
        std::unordered_map<uint32_t, uint16_t> code_to_dist_symbol;

        // Initialize mappings

        for (int i = 0; i < litlength_code_lengths.size(); i++) {
            if (litlength_code_lengths.at(i) > 0) {
                code_to_litlength_symbol.insert({(litlength_codes.at(i) << 16) | litlength_code_lengths.at(i), i});
            }
        }

        for (int i = 0; i < distance_code_lengths.size(); i++) {
            if (distance_code_lengths.at(i) > 0) {
                code_to_dist_symbol.insert({(distance_codes.at(i) << 16) | distance_code_lengths.at(i), i});
            }
        }

        // Finally, decode encoded data

        while (!bitstream.is_empty()) {
            uint16_t cur_code = 0;
            uint8_t cur_code_length = 0;

            // Check if code exists, and if the code length calculated matches the current code's length
            while (code_to_litlength_symbol.find((cur_code << 16) | cur_code_length) == code_to_litlength_symbol.end()) {
                cur_code = (cur_code << 1) | bitstream.pop();
                cur_code_length++;
            }
            
            uint16_t cur_symbol = code_to_litlength_symbol.at((cur_code << 16) | cur_code_length);

            if (cur_symbol <= 255) {
                // Literal
                decompressed.push_back(cur_symbol);
            } else if (cur_symbol > 256) {
                // This is a (length, distance) pair 

                // Find total length

                uint16_t total_length = FIXED_HUFFMAN_LENGTH_BASE.at(cur_symbol);
                uint8_t length_offset = bitstream.pop_num(get_bits_for_length_code(cur_symbol));

                total_length += length_offset;

                // Find total distance

                uint16_t cur_dist_code = 0;
                uint8_t cur_dist_code_length = 0;

                while (code_to_dist_symbol.find((cur_dist_code << 16) | cur_dist_code_length) == code_to_dist_symbol.end()) {
                    cur_dist_code = (cur_dist_code << 1) | bitstream.pop();
                    cur_dist_code_length++;
                }

                uint8_t dist_symbol = code_to_dist_symbol.at((cur_dist_code << 16) | cur_dist_code_length);

                uint16_t total_dist = FIXED_HUFFMAN_DIST_BASE.at(dist_symbol);
                uint8_t dist_offset_bits = FIXED_HUFFMAN_DIST_NUM_BITS.at(dist_symbol);
                
                if (dist_offset_bits == 0) {
                    total_dist = (dist_symbol + 1);
                } else {
                    uint16_t dist_offset = bitstream.pop_num(dist_offset_bits);

                    total_dist += dist_offset;
                }

                // Add bytes to stream from (length, distance) pair

                int start_pos = decompressed.size() - total_dist;

                for (int i = 0; i < total_length; i++) {
                    decompressed.push_back(decompressed.at(start_pos + i));
                }

            } else {
                break;
            }
        }


    } else {
        // BTYPE should not be 3 in a valid PNG file
        return;
    }
}