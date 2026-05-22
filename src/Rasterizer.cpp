
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <array>
#include <limits>
#include <memory>

#include "rasterizer/Rasterizer.h"
#include "utils/Visitor.h"
#include "FrameBuffer.h"
#include "primitives/Vertex.h"

void Rasterizer::plot_line_low(int x0, int y0, int x1, int y1, Color color, uint32_t z_index) {
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
        fragments.push_back(Fragment(x, y, color, z_index));

        if (D > 0) {
            y += yi;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2 * dy;
        }
    }
}

void Rasterizer::plot_line_high(int x0, int y0, int x1, int y1, Color color, uint32_t z_index) {
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
        fragments.push_back(Fragment(x, y, color, z_index));

        if (D > 0) {
            x += xi;
            D = D + (2 * (dx - dy));
        } else {
            D = D + 2 * dx;
        }
    }
}

void Rasterizer::fill_bottom_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, Color color, uint32_t z_index) {
    // Assumes v2.y == v3.y

    double invslope1 = (v2.pos.x - v1.pos.x) / (v2.pos.y - v1.pos.y);
    double invslope2 = (v3.pos.x - v1.pos.x) / (v3.pos.y - v1.pos.y);

    double curx1 = v1.pos.x;
    double curx2 = v1.pos.x;

    for (int scanlineY = v1.pos.y; scanlineY <= v2.pos.y; scanlineY++) {
        Primitive p(PRIMITIVE_TYPE::LINE, Vertex(curx1, scanlineY), Vertex(curx2, scanlineY));
        p.z_index = z_index;
        p.color = color;
        
        this->make_line_fragments(p);
        
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

void Rasterizer::fill_top_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, Color color, uint32_t z_index) {
    // Assumes v1.y == v2.y

    double invslope1 = (v3.pos.x - v1.pos.x) / (v3.pos.y - v1.pos.y);
    double invslope2 = (v3.pos.x - v2.pos.x) / (v3.pos.y - v2.pos.y);

    double curx1 = v3.pos.x;
    double curx2 = v3.pos.x;

    for (int scanlineY = v3.pos.y; scanlineY > v1.pos.y; scanlineY--) {
        Line p(Vertex(curx1, scanlineY), Vertex(curx2, scanlineY));
        p.z_index = z_index;
        p.color = color;

        this->make_line_fragments(p);

        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void Rasterizer::make_triangle_fragments(const Triangle& triangle) {
    if (triangle.v2.pos.y == triangle.v3.pos.y) {
        this->fill_bottom_flat_triangle(v1, v2, v3, triangle.color, triangle.get_z_index());
    } else if (v1.pos.y == triangle.v2.pos.y) {
        this->fill_top_flat_triangle(v1, v2, v3, triangle.color, triangle.get_z_index());
    } else {
        Vertex v4 = Vertex((int)(v1.pos.x + ((float)(v2.pos.y - v1.pos.y) / (float)(v3.pos.y - v1.pos.y)) * (v3.pos.x - v1.pos.x)), v2.pos.y);
        this->fill_bottom_flat_triangle(v1, v2, v4, triangle.color, triangle.get_z_index());
        this->fill_top_flat_triangle(v2, v4, v3, triangle.color, triangle.get_z_index());
    }


}

void Rasterizer::make_line_fragments(const Line& line) {
    int x0 = line.get_start_pos().x;
    int y0 = line.get_start_pos().y;
    int x1 = line.get_end_pos().x;
    int y1 = line.get_end_pos().y;

    if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
        if (x0 > x1) {
            this->plot_line_low(x1, y1, x0, y0, line.color, line.z_index);
        } else {
            this->plot_line_low(x0, y0, x1, y1, line.color, line.z_index);
        }
    } else {
        if (y0 > y1) {
            this->plot_line_high(x1, y1, x0, y0, line.color, line.z_index);
        } else {
            this->plot_line_high(x0, y0, x1, y1, line.color, line.z_index);
        }
    }
}

void Rasterizer::make_fragments(const std::vector<std::unique_ptr<Renderable>>& renderables) {
    for (const std::unique_ptr<Renderable>& r : renderables) {
        r.get()->rasterize(this->fragments);
    }
}

void Rasterizer::render_fragments(FrameBuffer& buffer) const {
    // Map from pixel position to maximum z index of fragment
    const size_t buffer_size = buffer.get_frame_buffer().size();
    std::vector<uint32_t> z_indices(buffer_size, std::numeric_limits<uint32_t>::min());

    for (const Fragment& f : this->fragments) {
        uint32_t fragment_index = buffer.get_width() * f.y + f.x;
        if (buffer.is_in_bounds(f.x, f.y) && f.z_index > z_indices.at(fragment_index)) {
            z_indices.at(fragment_index) = f.z_index;
            buffer.update_pixel(f.x, f.y, f.color);
        }
    }
}

void Rasterizer::reset() {
    this->fragments.clear();
}