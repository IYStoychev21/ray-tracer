#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Sphere
{
    glm::vec3 Position;
    float Radius;
    glm::vec3 Albedo;
};

struct Scene
{
    std::vector<Sphere> Spheres;
};