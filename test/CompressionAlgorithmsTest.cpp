
#include "doctest.h"
#include "image/CompressionAlgorithms.h"

TEST_CASE("DEFLATE decompression") {

    SUBCASE("Dynamic huffman tree compression") {

        SUBCASE("Data test 1") {

            std::vector<uint8_t> compressed = { 0x4B, 0x4C, 0x4A, 0x06, 0x00 };

            std::vector<uint8_t> decompressed;

            CompressionAlgorithms::DEFLATE_Decompress(compressed, decompressed);

            std::vector<uint8_t> expected = { 0x61, 0x62, 0x63 };

            REQUIRE(decompressed.size() == expected.size());

            for (int i = 0; i < decompressed.size(); i++) {
                INFO("Check failed at ", i, " Expected: ", expected.at(i), " Found: ", decompressed.at(i));
                CHECK(decompressed.at(i) == expected.at(i));
            }

        }

        SUBCASE("Data test 2") {

            std::vector<uint8_t> compressed = {
                0xED, 0xC1, 0x31, 0x01, 0x00, 0x00, 0x0C, 0x02, 
                0xA0, 0x6C, 0xDA, 0x3F, 0x94, 0x31, 0xB6, 0x03, 
                0x48, 0x00, 0x00, 0x00, 0x80, 0x6B, 0xE5, 0x95, 
                0x01
            };

            std::vector<uint8_t> decompressed;

            CompressionAlgorithms::DEFLATE_Decompress(compressed, decompressed);

            std::vector<uint8_t> expected;

            for (int i = 0; i < 4000; i++) {
                expected.push_back('A');
            }

            for (int i = 0; i < 400; i++) {
                expected.push_back('B');
            }

            REQUIRE(decompressed.size() == expected.size());

            for (int i = 0; i < decompressed.size(); i++) {
                INFO("Check failed at ", i, " Expected: ", expected.at(i), " Found: ", decompressed.at(i));
                CHECK(decompressed.at(i) == expected.at(i));
            }

        }

    }

}