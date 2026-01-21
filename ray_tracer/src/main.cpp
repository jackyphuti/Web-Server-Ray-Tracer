#include "ray_tracer.h"
#include "scene.h"
#include "texture.h"
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    int width = 800;
    int height = 600;
    int samples = 10;
    int num_threads = std::thread::hardware_concurrency();

    // Parse command line arguments
    if (argc > 1) width = std::stoi(argv[1]);
    if (argc > 2) height = std::stoi(argv[2]);
    if (argc > 3) samples = std::stoi(argv[3]);
    if (argc > 4) num_threads = std::stoi(argv[4]);

    std::cout << "==== C++ Advanced Ray Tracer ====\n";
    std::cout << "Resolution: " << width << "x" << height << "\n";
    std::cout << "Samples per pixel: " << samples << "\n";
    std::cout << "Threads: " << num_threads << "\n";
    std::cout << "Features: Reflections, Soft Shadows, Textures, Parallelization\n";

    try {
        RayTracer tracer(width, height, samples);
        tracer.set_num_threads(num_threads);
        tracer.set_max_depth(3);

        // Create a complex scene with various materials and effects
        Scene scene;

        // Ground plane (checkerboard texture, reflective)
        Material ground_material(Vector3(0.8, 0.8, 0.8), 0.15);
        ground_material.texture = std::make_shared<Texture>(
            Texture::Type::CHECKERBOARD,
            Vector3(1.0, 1.0, 1.0),
            Vector3(0.2, 0.2, 0.2)
        );
        scene.add_sphere(Sphere(Vector3(0, -101, -5), 100, ground_material));

        // Red sphere (matte)
        Material red_material(Vector3(1, 0.2, 0.2), 0.0);
        scene.add_sphere(Sphere(Vector3(-1.5, 0, -4), 1.0, red_material));

        // Green sphere (reflective)
        Material green_material(Vector3(0.2, 1, 0.2), 0.4);
        scene.add_sphere(Sphere(Vector3(0, 0, -5), 1.0, green_material));

        // Blue sphere (highly reflective)
        Material blue_material(Vector3(0.2, 0.2, 1), 0.7);
        scene.add_sphere(Sphere(Vector3(1.5, 0, -6), 1.0, blue_material));

        // Mirror sphere
        Material mirror_material(Vector3(1, 1, 1), 0.95);
        scene.add_sphere(Sphere(Vector3(0, 1.2, -7), 0.8, mirror_material));

        // Add lights with soft shadows
        scene.add_light(Light(Vector3(3, 3, -2), Vector3(1, 1, 1), 0.3));
        scene.add_light(Light(Vector3(-3, 2, -3), Vector3(0.5, 0.7, 1), 0.2));

        std::cout << "\nRendering scene with:\n";
        std::cout << "  - 5 spheres with different materials\n";
        std::cout << "  - 2 area lights for soft shadows\n";
        std::cout << "  - Reflections and textures\n";
        std::cout << "\nStarting render...\n";

        tracer.render_scene(scene, "output.ppm");
        std::cout << "\nSuccess! Image saved to output.ppm\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
