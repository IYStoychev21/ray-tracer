#pragma once
#include "Magenta.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
#include <imgui.h>

class Renderer
{
public:
    Renderer(Magenta::Application* app) : m_Application(app) { }
    ~Renderer() = default;

    void Render();
    void RenderUI();

private:
    bool HasResized();
    uint32_t RenderPixel(glm::vec2 coord);

private:
    std::shared_ptr<Magenta::Image> m_Image;
    glm::vec2 m_PrevSize = glm::vec2(0.0f);
    Magenta::Application* m_Application;
    std::vector<uint32_t> pixels;
};