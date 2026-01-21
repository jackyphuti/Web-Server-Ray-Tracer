#include "ray_tracer.h"
#include "scene.h"
#include "ppm_writer.h"
#include "texture.h"
#include <cmath>
#include <algorithm>
#include <random>
#include <thread>
#include <vector>
#include <iostream>

RayTracer::RayTracer(int width, int height, int samples_per_pixel)
    : width_(width), height_(height), samples_per_pixel_(samples_per_pixel) {
}

double RayTracer::random_float() const {
    static thread_local std::mt19937 gen(std::random_device{}());
    static thread_local std::uniform_real_distribution<> dis(0.0, 1.0);
    return dis(gen);
}

Vector3 RayTracer::random_on_sphere(double radius) const {
    double u = random_float();
    double v = random_float();
    double theta = 2.0 * 3.14159 * u;
    double phi = std::acos(2.0 * v - 1.0);
    
    double x = radius * std::sin(phi) * std::cos(theta);
    double y = radius * std::sin(phi) * std::sin(theta);
    double z = radius * std::cos(phi);
    
    return Vector3(x, y, z);
}

RayTracer::HitInfo RayTracer::check_intersection(const Vector3& origin, 
                                                  const Vector3& direction,
                                                  const Scene& scene) {
    HitInfo closest = {false, 1e10, Vector3(), Vector3(), nullptr};

    for (const auto& sphere : scene.get_spheres()) {
        // Ray-sphere intersection (quadratic equation)
        Vector3 oc = origin - sphere.center;
        double a = direction.dot(direction);
        double b = 2.0 * oc.dot(direction);
        double c = oc.dot(oc) - sphere.radius * sphere.radius;
        double discriminant = b * b - 4 * a * c;

        if (discriminant >= 0) {
            double t = (-b - std::sqrt(discriminant)) / (2 * a);
            if (t > 0.001 && t < closest.t) {
                closest.hit = true;
                closest.t = t;
                closest.point = origin + direction * t;
                closest.normal = (closest.point - sphere.center).normalize();
                closest.sphere = &sphere;
            }
        }
    }

    return closest;
}

Vector3 RayTracer::calculate_lighting(const HitInfo& hit, const Vector3& view_dir,
                                       const Scene& scene, int depth) {
    if (!hit.sphere) {
        return scene.get_background_color();
    }

    const auto& material = hit.sphere->material;
    Vector3 color(0, 0, 0);

    // Ambient light
    color = color + material.color * material.ambient;

    // Process each light
    for (const auto& light : scene.get_lights()) {
        Vector3 light_dir = (light.position - hit.point).normalize();
        
        // Soft shadow calculation
        double shadow_factor = 1.0;
        int shadow_samples = 5;
        
        for (int s = 0; s < shadow_samples; ++s) {
            Vector3 jittered_light = light.position + random_on_sphere(light.radius);
            Vector3 shadow_ray_dir = (jittered_light - hit.point).normalize();
            HitInfo shadow_hit = check_intersection(hit.point + hit.normal * 0.001, 
                                                   shadow_ray_dir, scene);
            
            if (!shadow_hit.hit) {
                shadow_factor += 1.0 / shadow_samples;
            }
        }

        // Diffuse shading
        double diffuse_intensity = std::max(0.0, hit.normal.dot(light_dir));
        color = color + material.color * light.intensity * 
                material.diffuse * diffuse_intensity * shadow_factor;

        // Specular shading (Blinn-Phong)
        Vector3 halfway = (light_dir - view_dir).normalize();
        double specular_intensity = std::pow(std::max(0.0, hit.normal.dot(halfway)), 
                                             material.shininess);
        color = color + light.intensity * material.specular * specular_intensity * shadow_factor;
    }

    return color;
}

Vector3 RayTracer::calculate_reflection(const HitInfo& hit, const Vector3& view_dir,
                                        const Scene& scene, int depth) {
    if (hit.sphere->material.reflection <= 0.0 || depth >= max_depth_) {
        return Vector3(0, 0, 0);
    }

    // Reflect ray
    Vector3 reflect_dir = view_dir - hit.normal * 2.0 * view_dir.dot(hit.normal);
    reflect_dir = reflect_dir.normalize();

    Vector3 reflected_color = cast_ray(hit.point + hit.normal * 0.001, 
                                       reflect_dir, scene, depth + 1);
    
    return reflected_color * hit.sphere->material.reflection;
}

Vector3 RayTracer::cast_ray(const Vector3& origin, const Vector3& direction,
                            const Scene& scene, int depth) {
    HitInfo hit = check_intersection(origin, direction, scene);

    if (!hit.hit) {
        return scene.get_background_color();
    }

    Vector3 view_dir = direction.normalize();
    Vector3 color = calculate_lighting(hit, view_dir, scene, depth);
    
    // Add reflection
    color = color + calculate_reflection(hit, view_dir, scene, depth);

    // Apply texture
    if (hit.sphere->material.texture) {
        Vector3 texture_color = hit.sphere->material.texture->get_color(0, 0, hit.point);
        color = color * texture_color;
    }

    return color;
}

void RayTracer::render_scene(const Scene& scene, const std::string& output_file) {
    PPMWriter writer(width_, height_);

    // Camera setup (simple perspective camera)
    Vector3 camera_pos(0, 1, 2);
    double fov = 60.0;
    double aspect_ratio = static_cast<double>(width_) / height_;
    double h = std::tan(fov * 3.14159 / 360.0);
    double w = h * aspect_ratio;

    std::cout << "Rendering with " << num_threads_ << " threads...\n";
    std::cout << "Max reflection depth: " << max_depth_ << "\n";

    // Parallel rendering using threads
    std::vector<std::thread> threads;
    int rows_per_thread = (height_ + num_threads_ - 1) / num_threads_;

    auto render_rows = [this, &writer, &scene, camera_pos, w, h, aspect_ratio]
                       (int start_y, int end_y) {
        for (int y = start_y; y < end_y && y < height_; ++y) {
            for (int x = 0; x < width_; ++x) {
                Vector3 color(0, 0, 0);

                // Multi-sampling for anti-aliasing
                for (int s = 0; s < samples_per_pixel_; ++s) {
                    double u = (2.0 * x - width_) / height_ * w;
                    double v = (height_ - 2.0 * y) / height_ * h;
                    
                    // Add jitter for anti-aliasing
                    u += (random_float() - 0.5) * 0.01;
                    v += (random_float() - 0.5) * 0.01;

                    Vector3 ray_dir = Vector3(u, v, -1).normalize();
                    color = color + cast_ray(camera_pos, ray_dir, scene);
                }

                color = color / samples_per_pixel_;
                writer.set_pixel(x, y, color);
            }
        }
    };

    // Create threads for parallel rendering
    for (int t = 0; t < num_threads_; ++t) {
        int start_y = t * rows_per_thread;
        int end_y = start_y + rows_per_thread;
        threads.emplace_back(render_rows, start_y, end_y);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "Writing image to " << output_file << "\n";
    writer.write(output_file);
}
