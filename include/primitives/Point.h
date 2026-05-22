
#pragma once

#include "primitives/Primitive.h"

class Point : public Primitive {
    private:
        Vertex p;
    public:
        Point(Vertex v) : p(v) {}

        Vector get_pos() const {
            return this->p.pos;
        }

        void apply_translation(double dx, double dy) override;

        void rasterize(std::vector<Fragment>& fragments) override;

        void apply_transformation_matrix(const Matrix& transformation_matrix) override;

        std::unique_ptr<Renderable> copy() const override;
};