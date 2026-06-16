
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

    while (e.is_active()) {
        e.tick();
    }

}