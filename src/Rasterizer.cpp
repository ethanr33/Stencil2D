
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <array>
#include <limits>
#include <memory>
#include <algorithm>

#include "rasterizer/Rasterizer.h"
#include "utils/Visitor.h"
#include "FrameBuffer.h"
#include "primitives/Vertex.h"


void Rasterizer::make_fragments(std::vector<std::unique_ptr<Renderable>>& renderables) {
    std::sort(renderables.begin(), renderables.end(), [](std::unique_ptr<Renderable>& a, std::unique_ptr<Renderable>& b) {
        return a->get_z_index() < b->get_z_index();
    });

    for (int i = 0; i < renderables.size(); i++) {
        renderables.at(i).get()->rasterize(this->fragments);
    }
}

void Rasterizer::render_fragments(FrameBuffer& buffer) {
    // fragments is guaranteed to be sorted by z index so pixels can be composited together from the bottom up

    for (int i = 0; i < fragments.size(); i++) {
        Fragment f = fragments.at(i);

        uint32_t fragment_index = buffer.get_width() * f.y + f.x;
        if (buffer.is_in_bounds(f.x, f.y)) {
            Color composed = Color::compose(buffer.get_pixel(f.x, f.y), f.color);
            buffer.update_pixel(f.x, f.y, composed);
        }
    }
}

void Rasterizer::reset() {
    this->fragments.clear();
}