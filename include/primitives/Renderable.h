
#pragma once

#include <vector>
#include <stdint.h>

#include "primitives/Vertex.h"
#include "utils/Color.h"
#include "rasterizer/Fragment.h"

enum class PRIMITIVE_TYPE { POINT, LINE, TRIANGLE };

class Renderable {
        private:
            // The higher the z index, the higher priority the primitive has
            uint32_t z_index;
        public:

            uint32_t get_z_index() const {
                return z_index;
            };

            void set_z_index(uint32_t z) {
                this->z_index = z;
            }

            /**
             * @brief Applies translation transformation renderable
             */
            virtual void apply_translation(double dx, double dy) = 0;

            /**
             * @brief Apply translation matrix transformations to renderable
             */
            virtual void apply_transformation_matrix(const Matrix& transformation_matrix) = 0;

            /**
             * @brief Generate fragments for renderable
             * 
             * @param fragments - Existing list of fragments to add to
             */
            virtual void rasterize(std::vector<Fragment>& fragments) = 0;

            virtual std::unique_ptr<Renderable> copy() const = 0;
};