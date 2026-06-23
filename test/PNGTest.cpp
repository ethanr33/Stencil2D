
#include <algorithm>

#include "doctest.h"
#include "image/Image.h"

TEST_CASE("PNG Tests") {

    SUBCASE("PNG Rasterization") {

        Image i("images/2x2.png");

        uint8_t z_index = 2;

        i.set_z_index(z_index);

        std::vector<Fragment> fragments;

        i.rasterize(fragments);

        REQUIRE(fragments.size() == 4);

        Fragment red(0, 0, Color(255, 0, 0), z_index);
        Fragment green(1, 0, Color(0, 255, 0), z_index);
        Fragment blue(0, 1, Color(0, 0, 255), z_index);
        Fragment yellow(1, 1, Color(255, 242, 0), z_index);


        CHECK(std::find(fragments.begin(), fragments.end(), red) != fragments.end());

    }

}