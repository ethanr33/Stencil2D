
#pragma once

#include "primitives/Primitive.h"

class Line : public Primitive {
    private:
        Vertex start;
        Vertex end;

        // Bresenham's line drawing algorithm helper functions
        void plot_line_low(int x0, int y0, int x1, int y1, std::vector<Fragment>& fragments);
        void plot_line_high(int x0, int y0, int x1, int y1, std::vector<Fragment>& fragments);
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

        std::unique_ptr<Renderable> copy() const override;
};