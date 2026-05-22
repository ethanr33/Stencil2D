
#include <iostream>
#include <chrono>

#include "Engine.h"
#include "transformations/TransformationManager.h"
#include "primitives/Point.h"

int main() {

    int width = 1920;
    int height = 1080;

    Engine e{width, height, true};

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Point p = Point(Vertex(j, i));
            p.set_color(Color(0, 255, 0));
            e.draw(p);
        }
    }

    while (e.is_active()) {
        e.tick();
    }

}