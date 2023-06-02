#pragma once
#include "Magenta.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
#include <imgui.h>

#include "Scene.h"
#include "Ray.h"

namespace utils
{
    uint32_t Vec4ToHex(glm::vec4& vec);
}

class Renderer
{
public:
    Renderer(Magenta::Application* app, std::shared_ptr<Scene> scene);
    

    ~Renderer() = default;

    void Render();
    void RenderUI();

private:
    bool HasResized();
    glm::vec4 RenderPixel(Ray& ray);

    void RenderViewportPanel();
    void RenderSettingsPanel();
    void RenderScenePanel();

private:
    std::shared_ptr<Magenta::Image> m_Image;
    glm::vec2 m_PrevSize = glm::vec2(0.0f);
    Magenta::Application* m_Application;
    std::vector<uint32_t> pixels;

    bool m_ShouldReRender = false;

    glm::vec3 m_PrevPos = glm::vec3(0.0f);

    std::shared_ptr<Scene> m_Scene;
};