#pragma once
#include "Magenta.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
#include <imgui.h>

#include "Camera.h"
#include "Ray.h"

namespace utils
{
    uint32_t Vec4ToHex(glm::vec4& vec);
}

class Renderer
{
public:
    Renderer(Magenta::Application* app, std::shared_ptr<Camera> cam) : m_Application(app) 
    { 
        m_Resolution = glm::vec2(m_Application->GetWidth(), m_Application->GetHeight());
        m_Camera = cam;
    }

    ~Renderer() = default;

    void Render();
    void RenderUI();

private:
    bool HasResized();
    glm::vec4 RenderPixel(glm::vec2 coord, Ray& ray);

private:
    std::shared_ptr<Magenta::Image> m_Image;
    glm::vec2 m_PrevSize = glm::vec2(0.0f);
    Magenta::Application* m_Application;
    std::vector<uint32_t> pixels;

    glm::vec2 m_Resolution;

    ImVec4 m_ImGuiColor = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);

    bool m_ShouldReRender = false;

    std::shared_ptr<Camera> m_Camera;
    glm::vec3 m_PrevPos = glm::vec3(0.0f);
};