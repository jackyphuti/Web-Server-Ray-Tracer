#pragma once

#include "vector3.h"
#include <memory>

class Scene;
class PPMWriter;

/**
 * RayTracer - Main ray tracing engine with:
 * - Ray-sphere intersection
 * - Reflection and refraction
 * - Soft shadows with area lights
 * - Multi-threaded rendering
 */
class RayTracer {
public:
    RayTracer(int width = 800, int height = 600, int samples_per_pixel = 10);

    // Render the scene and save to file
    void render_scene(const Scene& scene, const std::string& output_file);

    // Get dimensions
    int get_width() const { return width_; }
    int get_height() const { return height_; }
    int get_samples_per_pixel() const { return samples_per_pixel_; }

    // Set number of threads for parallel rendering
    void set_num_threads(int num_threads) { num_threads_ = num_threads; }

    // Set max reflection depth
    void set_max_depth(int depth) { max_depth_ = depth; }

private:
    int width_;
    int height_;
    int samples_per_pixel_;
    int max_depth_ = 3;
    int num_threads_ = 4;

    struct HitInfo {
        bool hit;
        double t;
        Vector3 point;
        Vector3 normal;
        const class Sphere* sphere;
    };

    // Ray casting with advanced lighting
    Vector3 cast_ray(const Vector3& origin, const Vector3& direction, 
                     const Scene& scene, int depth = 0);

    // Check ray-sphere intersection
    HitInfo check_intersection(const Vector3& origin, const Vector3& direction,
                               const Scene& scene);

    // Calculate lighting with soft shadows
    Vector3 calculate_lighting(const HitInfo& hit, const Vector3& view_dir,
                               const Scene& scene, int depth);

    // Calculate reflection
    Vector3 calculate_reflection(const HitInfo& hit, const Vector3& view_dir,
                                 const Scene& scene, int depth);

    // Generate random point on sphere (for soft shadows)
    Vector3 random_on_sphere(double radius) const;

    // Random number generator [0, 1)
    double random_float() const;
};
