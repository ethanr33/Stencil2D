
#pragma once

#include <memory>

#include "primitives/Renderable.h"

class Primitive : public Renderable {
    private:
        Color color;
    public:
        Color get_color() const {
            return color;
        }

        void set_color(const Color& c) {
            this->color = c;
        }

        virtual void apply_translation(double dx, double dy) = 0;

        virtual void rasterize(std::vector<Fragment>& fragments) = 0;

        virtual void apply_transformation_matrix(const Matrix& transformation_matrix) = 0;

        virtual std::unique_ptr<Renderable> copy() const = 0;
};
