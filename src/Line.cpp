
#include "primitives/Line.h"

void Line::plot_line_low(int x0, int y0, int x1, int y1, std::vector<Fragment>& fragments) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    
    int D = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        fragments.push_back(Fragment(x, y, this->get_color(), this->get_z_index()));

        if (D > 0) {
            y += yi;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2 * dy;
        }
    }
}

void Line::plot_line_high(int x0, int y0, int x1, int y1, std::vector<Fragment>& fragments) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    
    int D = 2 * dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++) {
        fragments.push_back(Fragment(x, y, this->get_color(), this->get_z_index()));

        if (D > 0) {
            x += xi;
            D = D + (2 * (dx - dy));
        } else {
            D = D + 2 * dx;
        }
    }
}


void Line::apply_translation(double dx, double dy) {
    this->start.pos.x += dx;
    this->start.pos.y += dy;

    this->end.pos.x += dx;
    this->end.pos.y += dy;
}

void Line::rasterize(std::vector<Fragment>& fragments) {
    int x0 = this->get_start_pos().x;
    int y0 = this->get_start_pos().y;
    int x1 = this->get_end_pos().x;
    int y1 = this->get_end_pos().y;

    if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
        if (x0 > x1) {
            this->plot_line_low(x1, y1, x0, y0, fragments);
        } else {
            this->plot_line_low(x0, y0, x1, y1, fragments);
        }
    } else {
        if (y0 > y1) {
            this->plot_line_high(x1, y1, x0, y0, fragments);
        } else {
            this->plot_line_high(x0, y0, x1, y1, fragments);
        }
    }
}

void Line::apply_transformation_matrix(const Matrix& transformation_matrix) {
    double new_x = transformation_matrix.get_element(0, 0) * this->start.pos.x + transformation_matrix.get_element(0, 1) * this->start.pos.y + transformation_matrix.get_element(0, 2);
    double new_y = transformation_matrix.get_element(1, 0) * this->start.pos.x + transformation_matrix.get_element(1, 1) * this->start.pos.y + transformation_matrix.get_element(1, 2);

    this->start.pos.x = new_x;
    this->start.pos.y = new_y;

    new_x = transformation_matrix.get_element(0, 0) * this->end.pos.x + transformation_matrix.get_element(0, 1) * this->end.pos.y + transformation_matrix.get_element(0, 2);
    new_y = transformation_matrix.get_element(1, 0) * this->end.pos.x + transformation_matrix.get_element(1, 1) * this->end.pos.y + transformation_matrix.get_element(1, 2);

    this->end.pos.x = new_x;
    this->end.pos.y = new_y;
}

std::unique_ptr<Renderable> Line::copy() const {
    return std::make_unique<Line>(*this);
}
