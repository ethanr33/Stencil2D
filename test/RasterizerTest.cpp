
// #include "doctest.h"
// #include "rasterizer/Rasterizer.h"

// TEST_CASE("Rasterize line") {

//     Rasterizer r;
//     FrameBuffer f(10, 10);

//     std::vector<Command> commands;

//     const Color red = Color(255, 0, 0);

//     SUBCASE("Same start and end point") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, 0), Vertex(0, 0));
//         p.color = red;

//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }

//     }

//     SUBCASE("Horizontal line") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, 0), Vertex(5, 0));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(1) = red;
//         expected.at(2) = red;
//         expected.at(3) = red;
//         expected.at(4) = red;
//         expected.at(5) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Vertical line") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, 0), Vertex(0, 5));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(10) = red;
//         expected.at(20) = red;
//         expected.at(30) = red;
//         expected.at(40) = red;
//         expected.at(50) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Vertical line with top clipping") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, -4), Vertex(0, 3));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(10) = red;
//         expected.at(20) = red;
//         expected.at(30) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Vertical line with bottom clipping") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, 1), Vertex(0, 30));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(10) = red;
//         expected.at(20) = red;
//         expected.at(30) = red;
//         expected.at(40) = red;
//         expected.at(50) = red;
//         expected.at(60) = red;
//         expected.at(70) = red;
//         expected.at(80) = red;
//         expected.at(90) = red;


//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Vertical line with top and bottom clipping") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, -1), Vertex(0, 30));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(10) = red;
//         expected.at(20) = red;
//         expected.at(30) = red;
//         expected.at(40) = red;
//         expected.at(50) = red;
//         expected.at(60) = red;
//         expected.at(70) = red;
//         expected.at(80) = red;
//         expected.at(90) = red;


//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at index ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Horizontal line with left clipping") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(-5, 0), Vertex(5, 0));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(1) = red;
//         expected.at(2) = red;
//         expected.at(3) = red;
//         expected.at(4) = red;
//         expected.at(5) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Horizontal line with right clipping") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, 0), Vertex(15, 0));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(1) = red;
//         expected.at(2) = red;
//         expected.at(3) = red;
//         expected.at(4) = red;
//         expected.at(5) = red;
//         expected.at(6) = red;
//         expected.at(7) = red;
//         expected.at(8) = red;
//         expected.at(9) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Horizontal line with right clipping") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(0, 0), Vertex(15, 0));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(1) = red;
//         expected.at(2) = red;
//         expected.at(3) = red;
//         expected.at(4) = red;
//         expected.at(5) = red;
//         expected.at(6) = red;
//         expected.at(7) = red;
//         expected.at(8) = red;
//         expected.at(9) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

//     SUBCASE("Horizontal line with both sides clipping") {
//         Primitive p(PRIMITIVE_TYPE::LINE, Vertex(-9, 0), Vertex(15, 0));
//         p.color = Color(255, 0, 0);


//         commands.push_back(AddPrimitiveCommand(p));

//         r.make_fragments(commands);
//         r.render_fragments(f);

//         std::vector<Color> expected(100);
//         expected.at(0) = red;
//         expected.at(1) = red;
//         expected.at(2) = red;
//         expected.at(3) = red;
//         expected.at(4) = red;
//         expected.at(5) = red;
//         expected.at(6) = red;
//         expected.at(7) = red;
//         expected.at(8) = red;
//         expected.at(9) = red;

//         for (int i = 0; i < 100; i++) {
//             INFO("Check failed at ", i, " Expected: ", expected.at(i).to_hex(), " Found: ", f.get_frame_buffer().at(i).to_hex());
//             CHECK(f.get_frame_buffer().at(i) == expected.at(i));
//         }
//     }

// }