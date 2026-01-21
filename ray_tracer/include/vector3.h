#pragma once

#include <cmath>
#include <iostream>

/**
 * Vector3 - 3D vector for graphics operations
 */
class Vector3 {
public:
    double x, y, z;

    // Constructors
    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

    // Operators
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(double scalar) const;
    Vector3 operator*(const Vector3& other) const;  // Component-wise multiplication
    Vector3 operator/(double scalar) const;

    // Dot product
    double dot(const Vector3& other) const;

    // Cross product
    Vector3 cross(const Vector3& other) const;

    // Length
    double length() const;
    double length_squared() const;

    // Normalize
    Vector3 normalize() const;

    // String representation
    std::string to_string() const;
};
