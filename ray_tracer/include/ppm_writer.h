#pragma once

#include "vector3.h"
#include <string>
#include <vector>

/**
 * PPMWriter - Writes image data to PPM (Portable Pixmap) format
 */
class PPMWriter {
public:
    PPMWriter(int width, int height);

    // Set pixel color at (x, y)
    void set_pixel(int x, int y, const Vector3& color);

    // Write image to file
    void write(const std::string& filename) const;

    // Get width and height
    int get_width() const { return width_; }
    int get_height() const { return height_; }

private:
    int width_;
    int height_;
    std::vector<Vector3> pixels_;

    // Convert color value to byte [0-255]
    int color_to_byte(double value) const;
};
