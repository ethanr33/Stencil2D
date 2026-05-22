
#include <iostream>
#include <chrono>

#include "Engine.h"
#include "transformations/TransformationManager.h"
#include "primitives/Triangle.h"

int main() {

    int width = 1920;
    int height = 1080;

    Engine e{width, height};



    Triangle p = Triangle(Vertex(0, 0), Vertex(500, 800), Vertex(1000, 1200));
    p.set_color(Color(255, 0, 0));

    e.draw(p);

    while (e.is_active()) {
        e.tick();
    }

}