#pragma once

#include "vector3.h"

/**
 * Texture - Simple procedural texture support
 */
class Texture {
public:
    enum class Type { SOLID, CHECKERBOARD, GRADIENT };

    Texture(Type type = Type::SOLID, const Vector3& color1 = Vector3(1, 1, 1),
            const Vector3& color2 = Vector3(0, 0, 0))
        : type_(type), color1_(color1), color2_(color2) {}

    Vector3 get_color(double u, double v, const Vector3& point) const;

private:
    Type type_;
    Vector3 color1_;
    Vector3 color2_;
};
