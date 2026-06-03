
#include <iostream>
#include <chrono>

#include "Engine.h"
#include "transformations/TransformationManager.h"
#include "image/Image.h"

int main() {

    int width = 1920;
    int height = 1080;

    Engine e{width, height, true};

    Image i("yellowbird-midflap.png");

    e.draw(i);

    while (e.is_active()) {
        e.tick();
    }

}