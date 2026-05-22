
#include "doctest.h"
#include "transformations/TransformationManager.h"
#include "primitives/Point.h"

TEST_CASE("No viewport transformations") {
    TransformationManager t;
    std::vector<std::unique_ptr<Renderable>> renderables;

    SUBCASE("Points") {

        SUBCASE("Single point") {
            Point p = Point(Vertex(2, 9));

            renderables.push_back(p.copy());

            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 1);

            const Point* transformed = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            REQUIRE(transformed != nullptr);
            CHECK(transformed->get_pos().x == 2);
            CHECK(transformed->get_pos().y == 9);
        }

        SUBCASE("Multiple points") {
            Point p1 = Point(Vertex(2, 9));
            Point p2 = Point(Vertex(20, 90));
            Point p3 = Point(Vertex(200, 900));


            renderables.push_back(p1.copy());
            renderables.push_back(p2.copy());
            renderables.push_back(p3.copy());


            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 3);

            const Point* transformed0 = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            const Point* transformed1 = dynamic_cast<const Point*>(t.get_transformed_renderables().at(1).get());
            const Point* transformed2 = dynamic_cast<const Point*>(t.get_transformed_renderables().at(2).get());
            REQUIRE(transformed0 != nullptr);
            REQUIRE(transformed1 != nullptr);
            REQUIRE(transformed2 != nullptr);
            CHECK(transformed0->get_pos().x == 2);
            CHECK(transformed0->get_pos().y == 9);
            CHECK(transformed1->get_pos().x == 20);
            CHECK(transformed1->get_pos().y == 90);
            CHECK(transformed2->get_pos().x == 200);
            CHECK(transformed2->get_pos().y == 900);
        }

    }

}

TEST_CASE("Simple translations") {

    TransformationManager t;
    std::vector<std::unique_ptr<Renderable>> renderables;

    SUBCASE("Points") {

        SUBCASE("Single point, single translation") {
            Point p = Point(Vertex(2, 9));

            renderables.push_back(p.copy());

            t.add_translation(19, 23);
            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 1);

            const Point* transformed = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            REQUIRE(transformed != nullptr);
            CHECK(transformed->get_pos().x == 21);
            CHECK(transformed->get_pos().y == 32);
        }

        SUBCASE("Single point, single floating point translation") {
            Point p = Point(Vertex(2, 9));

            renderables.push_back(p.copy());

            t.add_translation(0.2, 0.05);
            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 1);

            const Point* transformed = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            REQUIRE(transformed != nullptr);
            CHECK(transformed->get_pos().x == 2.2);
            CHECK(transformed->get_pos().y == 9.05);
        }

        SUBCASE("Single point, multiple translations") {
            Point p = Point(Vertex(2, 9));

            renderables.push_back(p.copy());

            t.add_translation(19, 23);
            t.add_translation(20, 22);
            t.add_translation(-2, -1);

            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 1);

            const Point* transformed = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            REQUIRE(transformed != nullptr);
            CHECK(transformed->get_pos().x == 39);
            CHECK(transformed->get_pos().y == 53);
        }

    }

    SUBCASE("Edge cases") {

        SUBCASE("Very large positive translation") {
            Point p = Point(Vertex(2, 9));

            renderables.push_back(p.copy());

            t.add_translation(1000000, 1000000);

            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 1);

            const Point* transformed = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            REQUIRE(transformed != nullptr);
            CHECK(transformed->get_pos().x == 1000002);
            CHECK(transformed->get_pos().y == 1000009);
        }

        SUBCASE("Very large negative translation") {
            Point p = Point(Vertex(2, 9));

            renderables.push_back(p.copy());

            t.add_translation(-1000000, -1000000);

            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 1);

            const Point* transformed = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            REQUIRE(transformed != nullptr);
            CHECK(transformed->get_pos().x == -999998);
            CHECK(transformed->get_pos().y == -999991);
        }

        SUBCASE("Zero translation") {
            Point p = Point(Vertex(2, 9));

            renderables.push_back(p.copy());

            t.add_translation(0, 0);

            t.apply_transformations(renderables);

            REQUIRE(renderables.size() == 1);

            const Point* transformed = dynamic_cast<const Point*>(t.get_transformed_renderables().at(0).get());
            REQUIRE(transformed != nullptr);
            CHECK(transformed->get_pos().x == 2);
            CHECK(transformed->get_pos().y == 9);
        }

    }

}
