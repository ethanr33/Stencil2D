
#pragma once

#include <algorithm>

#include "primitives/Primitive.h"

class Triangle : public Primitive {
    private:
        // For ease of use by renderer, v1.y <= v2.y <= v3.y
        Vertex v1;
        Vertex v2;
        Vertex v3;

        // Helper functions for standard triangle rasterization algorithm
        void fill_bottom_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<Fragment>& fragments);
        void fill_top_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<Fragment>& fragments);
    public:

        Triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
            std::vector<Vertex> vertices = {a, b, c};

            // Make sure to organize vertices in ascending y position

            std::sort(vertices.begin(), vertices.end(), [](Vertex a1, Vertex a2) {
                return a1.pos.y < a2.pos.y;
            });

            v1 = vertices.at(0);
            v2 = vertices.at(1);
            v3 = vertices.at(2);
        }

        Vector get_v1_pos() const {
            return v1.pos;
        }

        Vector get_v2_pos() const {
            return v2.pos;
        }

        Vector get_v3_pos() const {
            return v3.pos;
        }

        void rasterize(std::vector<Fragment>& fragments) override;

        void apply_transformation_matrix(const Matrix& transformation_matrix) override;

        std::unique_ptr<Renderable> copy() const override;
};