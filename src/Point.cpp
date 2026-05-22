
#include "primitives/Point.h"

void Point::apply_translation(double dx, double dy) {
    this->p.pos.x += dx;
    this->p.pos.y += dy;
};

void Point::rasterize(std::vector<Fragment>& fragments) {
    fragments.push_back(Fragment(p.pos.x, p.pos.y, this->get_color(), this->get_z_index()));
}

void Point::apply_transformation_matrix(const Matrix& transformation_matrix) {
    double new_x = transformation_matrix.get_element(0, 0) * this->p.pos.x + transformation_matrix.get_element(0, 1) * this->p.pos.y + transformation_matrix.get_element(0, 2);
    double new_y = transformation_matrix.get_element(1, 0) * this->p.pos.x + transformation_matrix.get_element(1, 1) * this->p.pos.y + transformation_matrix.get_element(1, 2);

    this->p.pos.x = new_x;
    this->p.pos.y = new_y;
}

std::unique_ptr<Renderable> Point::copy() const {
    return std::make_unique<Point>(*this);
}