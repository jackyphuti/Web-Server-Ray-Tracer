#include "ppm_writer.h"
#include <fstream>
#include <algorithm>
#include <cmath>

PPMWriter::PPMWriter(int width, int height)
    : width_(width), height_(height), pixels_(width * height, Vector3(0, 0, 0)) {
}

void PPMWriter::set_pixel(int x, int y, const Vector3& color) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_) {
        pixels_[y * width_ + x] = color;
    }
}

int PPMWriter::color_to_byte(double value) const {
    // Clamp to [0, 1]
    value = std::max(0.0, std::min(1.0, value));
    // Apply gamma correction
    value = std::sqrt(value);
    // Convert to [0, 255]
    return static_cast<int>(value * 255.99);
}

void PPMWriter::write(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // PPM header
    file << "P3\n";
    file << width_ << " " << height_ << "\n";
    file << "255\n";

    // Write pixels
    for (const auto& pixel : pixels_) {
        file << color_to_byte(pixel.x) << " "
             << color_to_byte(pixel.y) << " "
             << color_to_byte(pixel.z) << "\n";
    }

    file.close();
}
