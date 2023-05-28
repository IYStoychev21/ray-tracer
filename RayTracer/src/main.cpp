#include "Magenta.h"
#include "EntryPoint.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

#include "Renderer.h"

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

    void OnAttach() override 
    {
        m_Renderer = std::shared_ptr<Renderer>(new Renderer(m_Application));
        m_InputManager = m_Application->GetInputManager();
    }

    void OnDetach() override {}

    void OnUpdate() override 
    {
        if (m_InputManager->IsKeyDown(Magenta::KeyCode::Escape))
            m_Application->CloseWindow();

        m_Renderer->Render(); 
    }

    void OnUIRender() override 
    {
        m_Renderer->RenderUI(); 
    }
private:
    std::shared_ptr<Renderer> m_Renderer;
    std::shared_ptr<Magenta::InputManager> m_InputManager;
};

Magenta::Application* Magenta::CreateApplication()
{
    RayTracer* app = new RayTracer();

    app->InitWindow(1280, 720, "Ray Tracer");
    app->PushMagentaLayer(std::shared_ptr<MainLayer>(new MainLayer(app)));

    return app;
}