#pragma once

#include <chrono>

#include "Renderer.h"
#include "window/ApplicationWindow.h"
#include "primitives/Renderable.h"

/**
 * @brief Data structure for recording performance stats of engine
 */
struct EnginePerformanceStats {
    uint64_t num_frames = 0;
    uint64_t elapsed_time_ms = 0;
};

class Engine {
    private:
        Renderer renderer;
        ApplicationWindow app_window;

        // Optionally record performance data of engine and output to file when engine is destroyed
        bool performance_recording_enabled = false;
        std::string performance_file_data_path = "";
        std::string performance_file_data_name = "stats.txt";
        EnginePerformanceStats perf_stats;

        // Time at which object was initialized
        std::chrono::high_resolution_clock::time_point init_time;
    public:
        Engine(int width, int height, bool performance_recording_enabled = false) : renderer(width, height), app_window(width, height, "Default Title", renderer.get_front_buffer()),
        performance_recording_enabled(performance_recording_enabled) {

            app_window.init();
            
            if (this->performance_recording_enabled) {
                this->init_time = std::chrono::high_resolution_clock::now();
            }
        }

        /**
         * @brief Moves renderer state forward
         */
        void tick();

        void draw(const Renderable&);

        /**
         * @brief Clears all rendered entities
         */
        void clear();

        /**
         * @brief Translates viewport by (dx, dy) screen pixels
         * Essentially changes the top left corner coordinates from (x, y) to (x + dx, y + dy)
         */
        void translate_viewport(double, double);

        /**
         * @brief Gets if engine is active and running
         */
        bool is_active();

        /**
         * @brief Adds a event handler for the corresponding event type
         */
        void add_mouse_event_handler(MOUSE_EVENT_TYPE, std::function<void(MouseState)>);

        ~Engine();
};