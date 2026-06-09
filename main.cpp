
#include <iostream>
#include <chrono>

#include "Engine.h"
#include "transformations/TransformationManager.h"
#include "image/Image.h"

int main() {

    int width = 700;
    int height = 700;

    Engine e{width, height, true};

    Image i("background-day.png");

    e.draw(i);

    while (e.is_active()) {
        e.tick();
    }

}