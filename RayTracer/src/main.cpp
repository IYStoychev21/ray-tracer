#include "Magenta.h"
#include "EntryPoint.h"

#include <memory>
#include <vector>
#include <cstdint>
#include <glm/glm.hpp>
#include <iostream>

#include "Renderer.h"
#include "Scene.h"

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
        m_Scene = std::shared_ptr<Scene>(new Scene());

        m_Scene->Materials.push_back(Material{{1.0f, 0.0f, 1.0f}, 0.0f, 0.0f});
        m_Scene->Materials.push_back(Material{{0.3f, 0.2f, 0.8f}, 0.1f, 0.0f});
        m_Scene->Materials.push_back(Material{{0.3f, 0.8f, 0.2f}, 10.0f, 0.0f});

        m_Scene->Spheres.push_back(Sphere{{0.6f, 0.18f, 0.0f}, 0.5f, 0});
        m_Scene->Spheres.push_back(Sphere{{1.2f, -10.3f, 0.0f}, 10.0f, 1});
        m_Scene->Spheres.push_back(Sphere{{1.6f, 0.18f, 0.0f}, 0.3f, 2});
        
        m_Renderer = std::shared_ptr<Renderer>(new Renderer(m_Application, m_Scene));
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
    std::shared_ptr<Scene> m_Scene;
};

Magenta::Application* Magenta::CreateApplication()
{
    srand(time(0));
    RayTracer* app = new RayTracer();

    app->InitWindow(1280, 720, "Ray Tracer");
    app->PushMagentaLayer(std::shared_ptr<MainLayer>(new MainLayer(app)));

    return app;
}