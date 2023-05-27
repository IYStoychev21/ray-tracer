#include "Magenta.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

class RayTracer : public Magenta::Application
{
public:
    RayTracer() { }
    ~RayTracer() { }
};

class MainLayer : public Magenta::MagentaLayer
{
public:
    using MagentaLayer::MagentaLayer;

    void OnAttach() override {}

    void OnDetach() override {}

    void OnUpdate() override 
    {
       std::vector<uint32_t> imageData(m_Application->GetWidth() * m_Application->GetHeight()); 

       for (size_t i = 0; i < imageData.size(); i++)
       {
              imageData[i] = 0xffff00ff;
       }
       
       m_Image = std::make_shared<Magenta::Image>(imageData.data(), glm::vec2((float)m_Application->GetWidth(), (float)m_Application->GetHeight()), GL_RGBA);
    }

    void OnUIRender() override 
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(0, 0);

        ImGui::Begin("Viewport");

        ImGui::Image((void*)m_Image->GetTextureID(), ImGui::GetContentRegionAvail());

        ImGui::End();
    }
private:
    std::shared_ptr<Magenta::Image> m_Image;
};

Magenta::Application* Magenta::CreateApplication()
{
    RayTracer* app = new RayTracer();

    app->InitWindow(1280, 720, "Ray Tracer");
    app->PushMagentaLayer(std::shared_ptr<MainLayer>(new MainLayer(app)));

    return app;
}