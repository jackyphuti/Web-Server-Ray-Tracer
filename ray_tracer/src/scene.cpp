#include "scene.h"

Scene::Scene() : background_color_(0.1, 0.1, 0.1) {
}

void Scene::add_sphere(const Sphere& sphere) {
    spheres_.push_back(sphere);
}

void Scene::add_light(const Light& light) {
    lights_.push_back(light);
}
