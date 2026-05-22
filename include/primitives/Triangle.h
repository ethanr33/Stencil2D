
#pragma once

#include "primitives/Primitive.h"


class Triangle : public Primitive {
    private:
        // For ease of use by renderer, v1.y <= v2.y <= v3.y
        Vertex v1;
        Vertex v2;
        Vertex v3;
    public:

        Vector get_v1_pos() const {
            return v1.pos;
        }

        Vector get_v2_pos() const {
            return v2.pos;
        }

        Vector get_v3_pos() const {
            return v3.pos;
        }

        void apply_translation(double dx, double dy) override;

        void rasterize(std::vector<Fragment>& fragments) override;

        void apply_transformation_matrix(const Matrix& transformation_matrix) override;
};