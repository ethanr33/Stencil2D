
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
        if (buffer.is_in_bounds(f.x, f.y) && f.z_index >= z_indices.at(fragment_index)) {
            z_indices.at(fragment_index) = f.z_index;
            buffer.update_pixel(f.x, f.y, f.color);
        }
    }
}

void Rasterizer::reset() {
    this->fragments.clear();
}