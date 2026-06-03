
#pragma once

#include <string>

#include "primitives/Renderable.h"
#include "utils/Color.h"

class Image : public Renderable {
    private:
        // Enum representing the file format of this image
        enum class FILE_FORMAT { PNG, UNKNOWN };

        // Unscaled image width/heightr
        uint32_t base_width;
        uint32_t base_height;

        // Pixel coordinates of top left point of image
        Vector pos;

        // Vector containing image data
        // 1D so we can easily append colors once they are read from the file
        std::vector<Color> image_data;

        // Given a path to an image file, find what type of image it is
        // Let's not rely on the file extention and read the header instead, as file extensions can be arbitrary and changed by the user
        FILE_FORMAT determine_file_format(const std::string& file_path) const;

        // Constructor helper function, reads data from a PNG file into the current object
        void load_PNG(const std::string& file_path);
    public:

        Vector get_pos() const {
            return pos;
        }

        // Load image from file path
        Image(const std::string& file_path);

        void apply_transformation_matrix(const Matrix& transformation_matrix) override;

        void rasterize(std::vector<Fragment>& fragments) override;

        std::unique_ptr<Renderable> copy() const override;
};