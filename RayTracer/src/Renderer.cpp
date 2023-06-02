#include "Renderer.h"
#include <iostream>


glm::vec2 resolution;

Renderer::Renderer(Magenta::Application* app, std::shared_ptr<Scene> scene) : m_Application(app), m_Scene(scene)
{ 
    resolution = glm::vec2(m_Application->GetWidth(), m_Application->GetHeight());
}

void Renderer::Render()
{
    Ray ray;
    ray.Origin = {0.0f, 0.0f, 3.0f};

    if(m_PrevPos.x != ray.Origin.x || m_PrevPos.y != ray.Origin.y || m_PrevPos.z != ray.Origin.z || m_Application->GetInputManager()->IsMouseButtonDown(Magenta::MouseButton::Right))
    {
        m_ShouldReRender = true;
        m_PrevPos = ray.Origin;
    }

    if(!HasResized())
        return;

    pixels.clear();
    pixels.resize(m_Application->GetWidth() * m_Application->GetHeight());
    
    // m_Camera->Resize();

    for(uint32_t y = 0; y < m_Application->GetHeight(); y++)
    {
        for(uint32_t x = 0; x < m_Application->GetWidth(); x++)
        {
            // ray.Direction = m_Camera->GetRayDirections()[x + y * m_Application->GetWidth()];
            ray.Direction = glm::vec3(x / resolution.x, y / resolution.y, -1.0f);
            ray.Direction = ray.Direction * 2.0f - 1.0f;

            glm::vec4 color = RenderPixel(ray);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

            pixels[x + y * m_Application->GetWidth()] = utils::Vec4ToHex(color); 
        }
    } 

    m_Image = std::shared_ptr<Magenta::Image>(new Magenta::Image(pixels.data(), glm::vec2(m_Application->GetWidth(), m_Application->GetHeight()), GL_RGBA));
}

glm::vec4 Renderer::RenderPixel(Ray& ray)
{
    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // a = ray origin
    // b = ray direction
    // r = sphere radius
    // t = distance along ray

    if (m_Scene->Spheres.size() == 0)
        return {0.0f, 0.0f, 0.0f, 1.0f}; 

    
    Sphere* closestShepere = nullptr;
    float closestDistance = FLT_MAX;
    
    for(size_t i = 0; i < m_Scene->Spheres.size(); i++)
    {
        glm::vec3 origin = ray.Origin - m_Scene->Spheres[i].Position;

        float a = glm::dot(ray.Direction, ray.Direction);
        float b = 2.0f * glm::dot(ray.Direction, origin);
        float c = glm::dot(origin, origin) - glm::pow(m_Scene->Spheres[i].Radius, 2.0f);

        float discriminant = glm::pow(b, 2.0f) - 4.0f * a * c;

        if(discriminant < 0.0f)
            continue;

        // float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
        float t1 = (-b - glm::sqrt(discriminant)) / (2.0f * a);

        if(t1 < closestDistance)
        {
            closestShepere = &m_Scene->Spheres[i];
            closestDistance = t1;
        }
    }

    if(closestShepere == nullptr)
        return {0.0f, 0.0f, 0.0f, 1.0f};


    glm::vec3 origin = ray.Origin - closestShepere->Position;

    glm::vec3 hitPoint = origin + ray.Direction * closestDistance;

    glm::vec3 normal = glm::normalize(hitPoint);
    glm::vec3 lightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

    float d = glm::dot(normal, -lightDirection);

    return {closestShepere->Albedo * d, 1.0f};
}

bool Renderer::HasResized()
{
    if(m_PrevSize.x != resolution.x || m_PrevSize.y != resolution.y || m_ShouldReRender)
    {
        m_PrevSize = glm::vec2(resolution.x, resolution.y);
        m_ShouldReRender = false;
        return true;
    }

    return false;
}

void Renderer::RenderUI()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0, 0);
    RenderViewportPanel();

    style.WindowPadding = ImVec2(10, 10);
    RenderSettingsPanel();
    RenderScenePanel(); 
}

uint32_t utils::Vec4ToHex(glm::vec4& vec)
{
    uint8_t r = (uint32_t)(vec.r * 255.0f);
    uint8_t g = (uint32_t)(vec.g * 255.0f);
    uint8_t b = (uint32_t)(vec.b * 255.0f);
    uint8_t a = (uint32_t)(vec.a * 255.0f);

    return (a << 24) | (b << 16) | (g << 8) | r;
}

void Renderer::RenderViewportPanel()
{
    ImGui::Begin("Viewport");

    resolution = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};

    ImGui::Image((void*)m_Image->GetTextureID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void Renderer::RenderScenePanel()
{

    ImGui::Begin("Scene");
    ImGui::Spacing();

    for(size_t i = 0; i < m_Scene->Spheres.size(); i++)
    {
        glm::vec3 prevColor = m_Scene->Spheres[i].Albedo;
        float prevRadius = m_Scene->Spheres[i].Radius;
        glm::vec3 prevPos = m_Scene->Spheres[i].Position;

        ImGui::PushID(i);

        ImGui::Text("Sphere %d", i + 1);
        ImGui::ColorEdit3("Sphere Color", &m_Scene->Spheres[i].Albedo[0]);
        ImGui::Spacing();
        ImGui::DragFloat("Sphere Radius", &m_Scene->Spheres[i].Radius, 0.1f);
        ImGui::Spacing();
        ImGui::DragFloat3("Sphere Position", &m_Scene->Spheres[i].Position[0], 0.1f);

        ImGui::Separator();

        if(prevColor != m_Scene->Spheres[i].Albedo || prevRadius != m_Scene->Spheres[i].Radius || prevPos != m_Scene->Spheres[i].Position)
            m_ShouldReRender = true;        

        ImGui::PopID();
    }

    ImGui::End();
}

void Renderer::RenderSettingsPanel()
{
    ImGui::Begin("Settings");

    // ImGui::Separator();
    // ImGui::Spacing();

    // ImGui::Text("Camera Speed");
    // ImGui::SliderFloat("Speed", &m_Camera->speed, 0.2f, 10.0f);

    ImGui::Separator();
    ImGui::Spacing();

    if(ImGui::Button("Render", ImVec2(200, 25)))
        m_ShouldReRender = true;

    ImGui::End();
}