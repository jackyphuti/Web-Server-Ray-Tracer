#pragma once

#include "vector3.h"
#include "texture.h"
#include <vector>
#include <memory>

/**
 * Material - Surface properties for objects
 */
struct Material {
    Vector3 color;
    double ambient = 0.1;
    double diffuse = 0.7;
    double specular = 0.2;
    double shininess = 32.0;
    double reflection = 0.0;  // 0.0 = no reflection, 1.0 = perfect mirror
    std::shared_ptr<Texture> texture;

    Material(const Vector3& col = Vector3(1, 1, 1), double refl = 0.0)
        : color(col), reflection(refl),
          texture(std::make_shared<Texture>(Texture::Type::SOLID, col)) {}
};

/**
 * Sphere - A sphere object with material properties
 */
struct Sphere {
    Vector3 center;
    double radius;
    Material material;

    Sphere(const Vector3& center, double radius, const Material& material)
        : center(center), radius(radius), material(material) {}
};

/**
 * Light - Point light in the scene
 */
struct Light {
    Vector3 position;
    Vector3 intensity;
    double radius;  // For soft shadows

    Light(const Vector3& pos, const Vector3& intensity, double radius = 0.1)
        : position(pos), intensity(intensity), radius(radius) {}
};

/**
 * Scene - Collection of objects and lights to render
 */
class Scene {
public:
    Scene();

    // Add sphere to scene
    void add_sphere(const Sphere& sphere);

    // Add light to scene
    void add_light(const Light& light);

    // Get spheres
    const std::vector<Sphere>& get_spheres() const { return spheres_; }

    // Get lights
    const std::vector<Light>& get_lights() const { return lights_; }

    // Get background color
    Vector3 get_background_color() const { return background_color_; }

private:
    std::vector<Sphere> spheres_;
    std::vector<Light> lights_;
    Vector3 background_color_;
};
