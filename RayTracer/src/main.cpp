#include "Magenta.h"
#include "EntryPoint.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>

#include "Renderer.h"
#include "Camera.h"

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
        m_Camera = std::shared_ptr<Camera>(new Camera(45.0f, 0.1f, 100.0f, m_Application));
        m_Renderer = std::shared_ptr<Renderer>(new Renderer(m_Application, m_Camera));
        m_InputManager = m_Application->GetInputManager();
    }

    void OnDetach() override {}

    void OnUpdate() override 
    {
        if (m_InputManager->IsKeyDown(Magenta::KeyCode::Escape))
            m_Application->CloseWindow();

        m_Renderer->Render(); 
        m_Camera->CameraUpdate();
    }

    void OnUIRender() override 
    {
        m_Renderer->RenderUI(); 
    }
private:
    std::shared_ptr<Renderer> m_Renderer;
    std::shared_ptr<Magenta::InputManager> m_InputManager;
    std::shared_ptr<Camera> m_Camera;
};

Magenta::Application* Magenta::CreateApplication()
{
    RayTracer* app = new RayTracer();

    app->InitWindow(1280, 720, "Ray Tracer");
    app->PushMagentaLayer(std::shared_ptr<MainLayer>(new MainLayer(app)));

    return app;
}