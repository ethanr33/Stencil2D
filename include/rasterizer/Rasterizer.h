
#pragma once

#include <vector>

#include "FrameBuffer.h"
#include "rasterizer/Fragment.h"
#include "primitives/Renderable.h"
#include "primitives/Primitive.h"

class Rasterizer {
    private:
        std::vector<Fragment> fragments;
    public:
        /**
         * @brief Turn a list of renderables into fragments, which which are then rendered onto the frame buffer.
         */
        void make_fragments(const std::vector<std::unique_ptr<Renderable>>&);

        /**
         * @brief Renders current fragments onto a frame buffer
         * 
         */
        void render_fragments(FrameBuffer&);

        /**
         * @brief Resets rasterizer state for next frame
         */
        void reset();
};