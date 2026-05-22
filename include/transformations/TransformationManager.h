
#pragma once

#include <vector>
#include <memory>

#include "primitives/Primitive.h"
#include "utils/Matrix.h"

class TransformationManager {
    private:
        // 3x3 augmented matrix 
        Matrix transformation_matrix;

        // Use a separate buffer for renderables with transformations applied
        // This is so that we don't apply the same transformation again after the original renderables have been transformed
        std::vector<std::unique_ptr<Renderable>> transformed_renderables;
    public:
        TransformationManager() : transformation_matrix(3, 3, true) {}

        void add_translation(double dx, double dy);

        void apply_transformations(const std::vector<std::unique_ptr<Renderable>>&);

        const std::vector<std::unique_ptr<Renderable>> get_transformed_renderables() const noexcept {
            return transformed_renderables;
        }

        void clear_transformed_commands() {
            this->transformed_renderables.clear();
        }

        /**
         * Resets transformations and clears transformed renderables
         */
        void reset();
};