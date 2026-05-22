
// #include "doctest.h"
// #include "transformations/TransformationManager.h"

// TEST_CASE("No viewport transformations") {
//     TransformationManager t;

//     SUBCASE("Points") {

//         SUBCASE("Single point") {
//             Primitive p = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));

//             commands.push_back(AddPrimitiveCommand(p));

//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 1);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == 2);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == 9);
//         }

//         SUBCASE("Multiple points") {
//             Primitive p1 = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));
//             Primitive p2 = Primitive(PRIMITIVE_TYPE::POINT, Vertex(20, 90));
//             Primitive p3 = Primitive(PRIMITIVE_TYPE::POINT, Vertex(200, 900));


//             commands.push_back(AddPrimitiveCommand(p1));
//             commands.push_back(AddPrimitiveCommand(p2));
//             commands.push_back(AddPrimitiveCommand(p3));


//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 3);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == 2);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == 9);
//             CHECK(t.get_transformed_commands().at(1).primitive.vertices[0].pos.x == 20);
//             CHECK(t.get_transformed_commands().at(1).primitive.vertices[0].pos.y == 90);
//             CHECK(t.get_transformed_commands().at(2).primitive.vertices[0].pos.x == 200);
//             CHECK(t.get_transformed_commands().at(2).primitive.vertices[0].pos.y == 900);
//         }

//     }

// }

// TEST_CASE("Simple translations") {

//     TransformationManager t;
//     std::vector<AddPrimitiveCommand> commands;

//     SUBCASE("Points") {

//         SUBCASE("Single point, single translation") {
//             Primitive p = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));

//             commands.push_back(AddPrimitiveCommand(p));

//             t.add_translation(19, 23);
//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 1);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == 21);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == 32);
//         }

//         SUBCASE("Single point, single floating point translation") {
//             Primitive p = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));

//             commands.push_back(AddPrimitiveCommand(p));

//             t.add_translation(0.2, 0.05);
//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 1);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == 2.2);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == 9.05);
//         }

//         SUBCASE("Single point, multiple translations") {
//             Primitive p = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));

//             commands.push_back(AddPrimitiveCommand(p));

//             t.add_translation(19, 23);
//             t.add_translation(20, 22);
//             t.add_translation(-2, -1);

//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 1);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == 39);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == 53);
//         }

//     }

//     SUBCASE("Edge cases") {

//         SUBCASE("Very large positive translation") {
//             Primitive p = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));

//             commands.push_back(AddPrimitiveCommand(p));

//             t.add_translation(1000000, 1000000);

//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 1);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == 1000002);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == 1000009);
//         }

//         SUBCASE("Very large negative translation") {
//             Primitive p = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));

//             commands.push_back(AddPrimitiveCommand(p));

//             t.add_translation(-1000000, -1000000);

//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 1);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == -999998);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == -999991);
//         }

//         SUBCASE("Zero translation") {
//             Primitive p = Primitive(PRIMITIVE_TYPE::POINT, Vertex(2, 9));

//             commands.push_back(AddPrimitiveCommand(p));

//             t.add_translation(0, 0);

//             t.apply_transformations(commands);

//             REQUIRE(commands.size() == 1);

//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.x == 2);
//             CHECK(t.get_transformed_commands().at(0).primitive.vertices[0].pos.y == 9);
//         }

//     }

// }