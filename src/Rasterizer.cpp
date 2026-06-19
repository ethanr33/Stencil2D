
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


void Rasterizer::make_fragments(const std::vector<std::unique_ptr<Renderable>>& renderables) {
    for (const std::unique_ptr<Renderable>& r : renderables) {
        r.get()->rasterize(this->fragments);
    }
}

void Rasterizer::render_fragments(FrameBuffer& buffer) {
    std::sort(fragments.begin(), fragments.end(), [](Fragment a, Fragment b) {
        return a.z_index < b.z_index;
    });

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