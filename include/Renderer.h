
#pragma once 

#include "primitives/Vertex.h"
#include "transformations/TransformationManager.h"
#include "rasterizer/Rasterizer.h"
#include "FrameBuffer.h"
#include "window/ApplicationWindow.h"

class Renderer {
    private:
        int display_width;
        int display_height;

        // List of object which are being drawn
        std::vector<std::unique_ptr<Renderable>> renderables;

        TransformationManager transformation_manager;
        Rasterizer rasterizer;
        
        FrameBuffer front_buffer;
        FrameBuffer back_buffer;

        // Has the renderer state been updated since last display() call?
        // Useful for avoiding unnecessary command/fragment processing
        bool state_updated = false;
    public:
        Renderer(int width, int height) : display_width(width), display_height(height), front_buffer(width, height), back_buffer(width, height) {}

        /**
         * @brief Clears back frame buffer, resets command stream
         */
        void clear();

        /**
         * @brief Adds a new renderable object to command stream
         */
        void add_renderable(const Renderable&);

        /**
         * @brief Translates viewport top left corner from (x, y) to (x + dx, y + dy)
         */
        void translate_absolute(double, double);

        /**
         * @brief Updates front frame buffer with the most recent primitives added since last clear()
        */
        void display();

        /**
         * @brief Gets front buffer for rendering
         */
        const FrameBuffer* get_front_buffer() const;
};