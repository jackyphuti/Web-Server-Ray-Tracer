#include "vector3.h"

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator*(const Vector3& other) const {
    return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 Vector3::operator/(double scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
}

double Vector3::dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(const Vector3& other) const {
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

double Vector3::length() const {
    return std::sqrt(length_squared());
}

double Vector3::length_squared() const {
    return x * x + y * y + z * z;
}

Vector3 Vector3::normalize() const {
    double len = length();
    if (len == 0) return Vector3(0, 0, 0);
    return *this / len;
}

std::string Vector3::to_string() const {
    return "(" + std::to_string(x) + ", " + 
           std::to_string(y) + ", " + 
           std::to_string(z) + ")";
}
