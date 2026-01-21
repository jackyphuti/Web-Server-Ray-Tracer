#include "texture.h"
#include <cmath>

Vector3 Texture::get_color(double u, double v, const Vector3& point) const {
    switch (type_) {
        case Type::SOLID:
            return color1_;

        case Type::CHECKERBOARD: {
            // Checkerboard pattern
            int scale = 10;
            int u_check = static_cast<int>(point.x * scale) % 2;
            int v_check = static_cast<int>(point.z * scale) % 2;
            return (u_check == v_check) ? color1_ : color2_;
        }

        case Type::GRADIENT: {
            // Linear gradient based on y coordinate
            double t = std::sin(point.y * 3.14159) * 0.5 + 0.5;
            return color1_ * (1 - t) + color2_ * t;
        }

        default:
            return color1_;
    }
}
