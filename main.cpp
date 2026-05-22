
#include <iostream>
#include <chrono>

#include "Engine.h"
#include "transformations/TransformationManager.h"
#include "primitives/Triangle.h"

int main() {

    int width = 1920;
    int height = 1080;

    Engine e{width, height};

    e.draw(p);

    while (e.is_active()) {
        e.tick();
    }

}