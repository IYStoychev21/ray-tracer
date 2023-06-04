#include "Renderer.h"
#include <iostream>
#include <glm/gtc/random.hpp>

glm::vec2 resolution;

Renderer::Renderer(Magenta::Application* app, std::shared_ptr<Scene> scene) : m_Application(app), m_Scene(scene)
{ 
    resolution = glm::vec2(m_Application->GetWidth(), m_Application->GetHeight());
}

void Renderer::Render()
{

    if(HasResized())
    {
        pixels.clear();
        pixels.resize(m_Application->GetWidth() * m_Application->GetHeight());

        m_AccumuletedPixels.clear();
        m_AccumuletedPixels.resize(m_Application->GetWidth() * m_Application->GetHeight());

        m_FrameIndex = 1;

        m_Horizontal.reserve(m_Application->GetWidth());
        m_Vertical.reserve(m_Application->GetHeight());

        for(int i = 0; i < m_Application->GetWidth(); i++)
            m_Horizontal.push_back(i);

        for(int i = 0; i < m_Application->GetHeight(); i++)
            m_Vertical.push_back(i);
    }
    
    // m_Camera->Resize();

    std::for_each(std::execution::par, m_Horizontal.begin(), m_Horizontal.end(), [this](int x)
    {
       std::for_each(std::execution::par, m_Vertical.begin(), m_Vertical.end(), [this, x](int y)
       {
           if(m_FrameIndex == 1)
               m_AccumuletedPixels[x + y * m_Application->GetWidth()] = glm::vec4(0.0f);

           glm::vec4 color = RayGen(x, y);

           m_AccumuletedPixels[x + y * m_Application->GetWidth()] += color;
           glm::vec4 accumulatedColor = m_AccumuletedPixels[x + y * m_Application->GetWidth()] / (float)m_FrameIndex;

           accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
           pixels[x + y * m_Application->GetWidth()] = utils::Vec4ToHex(accumulatedColor); 
       });
    });

    if (m_ShouldAccumulate)
        m_FrameIndex++;
    else
        m_FrameIndex = 1;

    m_Image = std::shared_ptr<Magenta::Image>(new Magenta::Image(pixels.data(), glm::vec2(m_Application->GetWidth(), m_Application->GetHeight()), GL_RGBA));
}

glm::vec4 Renderer::RayGen(uint32_t x, uint32_t y)
{
    x *= resolution.x / resolution.y;
    Ray ray;
    ray.Origin = glm::vec3(0.0f, 0.0f, 3.0f);
    ray.Direction = glm::vec3(x / resolution.x, y / resolution.y, -1.0f);
    ray.Direction = ray.Direction * 2.0f - 1.0f;

    float multiplyer = 1.0f;
    int bounceCount = 5;
    glm::vec4 color = glm::vec4(0.0f);

    for(int i = 0; i < bounceCount; i++)
    {
        RayData data = TraceRay(ray);

        if(data.HitDistance < 0.0f)
        {
            glm::vec3 skyColor = glm::vec3(0.6f, 0.7f, 0.9f);
            color += glm::vec4(skyColor * multiplyer, 1.0f);
            break;
        }

        glm::vec3 lightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
        float d = glm::dot(data.HitNormal, -lightDirection);

        Sphere& sphere = m_Scene->Spheres[data.HitObjectIndex];
        Material materail = m_Scene->Materials[sphere.MaterialIndex];

        glm::vec4 sphereColor = {materail.Albedo, 1.0f};
        sphereColor *= d;

        color += sphereColor * multiplyer;
        multiplyer *= 0.5f;

        ray.Origin = data.HitPosition + data.HitNormal * 0.0001f;
        ray.Direction = glm::reflect(ray.Direction, data.HitNormal + materail.Roughness * glm::linearRand(-0.5f, 0.5f));
    }

    return color;
}

Renderer::RayData Renderer::TraceRay(Ray& ray)
{
    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // a = ray origin
    // b = ray direction
    // r = sphere radius
    // t = distance along ray
    Renderer::RayData rayData;
    
    int32_t closestIndex = -1;
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

        if(t1 < closestDistance && t1 > 0.0f)
        {
            closestIndex = i;
            closestDistance = t1;
        }
    }

    if(closestIndex < 0)
        return MissObject(ray);

    return HitObject(closestIndex, ray, closestDistance);
}

Renderer::RayData Renderer::MissObject(Ray& ray)
{
    Renderer::RayData rayData;
    rayData.HitDistance = -1.0f;
    return rayData;
}

Renderer::RayData Renderer::HitObject(int32_t closestIndex, Ray& ray, float distance)
{
    Renderer::RayData rayData;
    rayData.HitObjectIndex = closestIndex;
    rayData.HitDistance = distance;

    Sphere& sphere = m_Scene->Spheres[closestIndex];

    glm::vec3 origin = ray.Origin - sphere.Position;
    rayData.HitPosition = origin + ray.Direction * distance;
    rayData.HitNormal = glm::normalize(rayData.HitPosition);
    rayData.HitPosition += sphere.Position;

    return rayData;
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

    ImGui::Text("Spheres");
    for(size_t i = 0; i < m_Scene->Spheres.size(); i++)
    {
        float prevRadius = m_Scene->Spheres[i].Radius;
        glm::vec3 prevPos = m_Scene->Spheres[i].Position;

        ImGui::PushID(i);

        ImGui::DragFloat("Sphere Radius", &m_Scene->Spheres[i].Radius, 0.05f);
        ImGui::Spacing();
        ImGui::DragFloat3("Sphere Position", &m_Scene->Spheres[i].Position[0], 0.05f);

        ImGui::Separator();

        if(prevRadius != m_Scene->Spheres[i].Radius || prevPos != m_Scene->Spheres[i].Position)
            m_ShouldReRender = true;        

        ImGui::PopID();
    }

    ImGui::Text("Materials");
    for(size_t i = 0; i < m_Scene->Materials.size(); i++)
    {
        glm::vec3 prevColor = m_Scene->Materials[i].Albedo;
        Material& material = m_Scene->Materials[i];
        float prevMetallic = material.Metallic;
        float prevRoughness = material.Roughness;

        ImGui::PushID(i);

        ImGui::Text("Materail %d", i + 1);
        ImGui::ColorEdit3("Albedo", &material.Albedo[0]);
        ImGui::Spacing();
        ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
        ImGui::Spacing();
        ImGui::DragFloat("Metallic", &material.Metallic, 0.1f);

        ImGui::Separator();

        if(prevColor != material.Albedo || prevMetallic != material.Metallic || prevRoughness != material.Roughness)
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

    ImGui::Checkbox("Acummulation", &m_ShouldAccumulate);
    if(ImGui::Button("Reset FrameIndex"))
        m_FrameIndex = 1;

    ImGui::Separator();
    ImGui::Spacing();

    if(ImGui::Button("Render", ImVec2(200, 25)))
        m_ShouldReRender = true;

    ImGui::End();
}