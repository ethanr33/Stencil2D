
#pragma once

#include <vector>

#include "FrameBuffer.h"
#include "rasterizer/Fragment.h"
#include "primitives/Renderable.h"
#include "primitives/Primitive.h"

class Rasterizer {
    private:
        std::vector<Fragment> fragments;

        // Helper functions for standard triangle rasterization algorithm
        void fill_bottom_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, Color, uint32_t z_index);
        void fill_top_flat_triangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, Color, uint32_t z_index);

    public:
        /**
         * @brief Turn a list of renderables into fragments, which which are then rendered onto the frame buffer.
         */
        void make_fragments(const std::vector<std::unique_ptr<Renderable>>&);

        /**
         * @brief Renders current fragments onto a frame buffer
         * 
         */
        void render_fragments(FrameBuffer&) const;

        /**
         * @brief Resets rasterizer state for next frame
         */
        void reset();
};