#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Material
{
    glm::vec3 Albedo;
    float Roughness;
    float Metallic;
};

struct Sphere
{
    glm::vec3 Position;
    float Radius;
    int32_t MaterialIndex;
};

struct Scene
{
    std::vector<Sphere> Spheres;
    std::vector<Material> Materials;
};