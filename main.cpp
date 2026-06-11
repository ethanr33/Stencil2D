
#include <iostream>
#include <chrono>

#include "Engine.h"
#include "transformations/TransformationManager.h"
#include "image/Image.h"

int main() {

    int width = 288;
    int height = 512;

    Engine e{width, height, true};

    Image bg("background-day.png");
    
    bg.set_z_index(1);

    e.draw(bg);

    uint32_t y_pos = 0;

    while (e.is_active()) {

        Image player("yellowbird-midflap.png");
        player.set_z_index(2);

        player.set_pos(Vector(0, y_pos));

        y_pos++;

        e.draw(bg);
        e.draw(player);

        e.tick();
    }

}