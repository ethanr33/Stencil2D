
#pragma once

#include "primitives/Primitive.h"

class Line : public Primitive {
    private:
        Vertex start;
        Vertex end;
    public:

        Line(Vertex start, Vertex end) : start(start), end(end) {}

        Vector get_start_pos() const {
            return this->start.pos;
        }

        Vector get_end_pos() const {
            return this->end.pos;
        }

        void apply_translation(double dx, double dy) override;

        void rasterize(std::vector<Fragment>& fragments) override;

        void apply_transformation_matrix(const Matrix& transformation_matrix) override;
};