
#include "primitives/Triangle.h"

void Triangle::fill_bottom_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<Fragment>& fragments) {
    // Assumes v2.y == v3.y

    double invslope1 = (v2.pos.x - v1.pos.x) / (v2.pos.y - v1.pos.y);
    double invslope2 = (v3.pos.x - v1.pos.x) / (v3.pos.y - v1.pos.y);

    double curx1 = v1.pos.x;
    double curx2 = v1.pos.x;

    for (int scanlineY = v1.pos.y; scanlineY <= v2.pos.y; scanlineY++) {

        for (int cur_x = curx1; cur_x <= curx2; cur_x++) {
            fragments.push_back(Fragment(cur_x, scanlineY, this->get_color(), this->get_z_index()));
        }
                
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void Triangle::fill_top_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, std::vector<Fragment>& fragments) {
    // Assumes v1.y == v2.y

    double invslope1 = (v3.pos.x - v1.pos.x) / (v3.pos.y - v1.pos.y);
    double invslope2 = (v3.pos.x - v2.pos.x) / (v3.pos.y - v2.pos.y);

    double curx1 = v3.pos.x;
    double curx2 = v3.pos.x;

    for (int scanlineY = v3.pos.y; scanlineY > v1.pos.y; scanlineY--) {
        for (int cur_x = curx1; cur_x <= curx2; cur_x++) {
            fragments.push_back(Fragment(cur_x, scanlineY, this->get_color(), this->get_z_index()));
        }

        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void Triangle::apply_translation(double dx, double dy) {
    this->v1.pos.x += dx;
    this->v1.pos.y += dy;

    this->v2.pos.x += dx;
    this->v2.pos.y += dy;

    this->v3.pos.x += dx;
    this->v3.pos.y += dy;
}

void Triangle::rasterize(std::vector<Fragment>& fragments) {
    if (this->v2.pos.y == this->v3.pos.y) {
        this->fill_bottom_flat_triangle(v1, v2, v3, fragments);
    } else if (this->v1.pos.y == this->v2.pos.y) {
        this->fill_top_flat_triangle(v1, v2, v3, fragments);
    } else {
        Vertex v4 = Vertex((int)(v1.pos.x + ((float)(v2.pos.y - v1.pos.y) / (float)(v3.pos.y - v1.pos.y)) * (v3.pos.x - v1.pos.x)), v2.pos.y);
        this->fill_bottom_flat_triangle(v1, v2, v4, fragments);
        this->fill_top_flat_triangle(v2, v4, v3, fragments);
    }
}

void Triangle::apply_transformation_matrix(const Matrix& transformation_matrix) {
    double new_x = transformation_matrix.get_element(0, 0) * this->v1.pos.x + transformation_matrix.get_element(0, 1) * this->v1.pos.y + transformation_matrix.get_element(0, 2);
    double new_y = transformation_matrix.get_element(1, 0) * this->v1.pos.x + transformation_matrix.get_element(1, 1) * this->v1.pos.y + transformation_matrix.get_element(1, 2);

    this->v1.pos.x = new_x;
    this->v1.pos.y = new_y;

    new_x = transformation_matrix.get_element(0, 0) * this->v2.pos.x + transformation_matrix.get_element(0, 1) * this->v2.pos.y + transformation_matrix.get_element(0, 2);
    new_y = transformation_matrix.get_element(1, 0) * this->v2.pos.x + transformation_matrix.get_element(1, 1) * this->v2.pos.y + transformation_matrix.get_element(1, 2);

    this->v2.pos.x = new_x;
    this->v2.pos.y = new_y;

    new_x = transformation_matrix.get_element(0, 0) * this->v3.pos.x + transformation_matrix.get_element(0, 1) * this->v3.pos.y + transformation_matrix.get_element(0, 2);
    new_y = transformation_matrix.get_element(1, 0) * this->v3.pos.x + transformation_matrix.get_element(1, 1) * this->v3.pos.y + transformation_matrix.get_element(1, 2);

    this->v3.pos.x = new_x;
    this->v3.pos.y = new_y;
}

std::unique_ptr<Renderable> Triangle::copy() const {
    return std::make_unique<Triangle>(*this);
}