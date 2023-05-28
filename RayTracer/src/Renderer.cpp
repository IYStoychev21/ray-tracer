#include "Renderer.h"
#include <iostream>

void Renderer::Render()
{
    if(HasResized())
    {
        pixels.clear();
        pixels.resize(m_Application->GetWidth() * m_Application->GetHeight());

        for(uint32_t y = 0; y < m_Application->GetHeight(); y++)
        {
            for(uint32_t x = 0; x < m_Application->GetWidth(); x++)
            {
                glm::vec2 textCoord = { (float)x / (float)m_Application->GetWidth(), (float)y / (float)m_Application->GetHeight() };
                textCoord = textCoord * 2.0f - 1.0f;

                glm::vec4 color = RenderPixel(textCoord);
                color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

                pixels[x + y * m_Application->GetWidth()] = utils::Vec4ToHex(color); 
            }
        } 

        m_Image = std::shared_ptr<Magenta::Image>(new Magenta::Image(pixels.data(), glm::vec2(m_Application->GetWidth(), m_Application->GetHeight()), GL_RGBA));
    }
}

glm::vec4 Renderer::RenderPixel(glm::vec2 coord)
{
    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // a = ray origin
    // b = ray direction
    // r = sphere radius
    // t = distance along ray

    float aspectRation = m_Resolution.x / m_Resolution.y;
    coord.x *= aspectRation;

    glm::vec3 rayDirection = glm::vec3(coord.x, coord.y, -1.0f);
    glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, 1.0f);

    float radius = 0.5f;

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayDirection, rayOrigin);
    float c = glm::dot(rayOrigin, rayOrigin) - glm::pow(radius, 2.0f);

    float discriminant = glm::pow(b, 2.0f) - 4.0f * a * c;

    if(discriminant < 0.0f)
        return {0.0f, 0.0f, 0.0f, 1.0f};

    float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
    float t1 = (-b - glm::sqrt(discriminant)) / (2.0f * a);

    glm::vec3 hitPoint = rayOrigin + rayDirection * t1;

    glm::vec3 normal = glm::normalize(hitPoint);
    glm::vec3 lightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));

    float d = glm::max(glm::dot(normal, -lightDirection), 0.0f);

    glm::vec3 color = {m_ImGuiColor.x, m_ImGuiColor.y, m_ImGuiColor.z};
    color *= d;
    return {color, 1.0f};
}

bool Renderer::HasResized()
{
    if(m_PrevSize.x != m_Resolution.x || m_PrevSize.y != m_Resolution.y || m_ShouldReRender)
    {
        m_PrevSize = glm::vec2(m_Resolution.x, m_Resolution.y);
        return true;
    }

    return false;
}

void Renderer::RenderUI()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0, 0);

    ImGui::Begin("Viewport");

    m_Resolution = {ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y};

    ImGui::Image((void*)m_Image->GetTextureID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    style.WindowPadding = ImVec2(10, 10);
    ImVec4 prevColor = m_ImGuiColor;

    ImGui::Begin("Settings");
    ImGui::ColorPicker3("Color", (float*)&m_ImGuiColor);

    ImGui::Spacing();

    if(ImGui::Button("Re-render", ImVec2(200, 25)))
        m_ShouldReRender = true;
    else if(prevColor.x != m_ImGuiColor.x || prevColor.y != m_ImGuiColor.y || prevColor.z != m_ImGuiColor.z)
        m_ShouldReRender = true;
    else
        m_ShouldReRender = false;


    ImGui::End();
}

uint32_t utils::Vec4ToHex(glm::vec4& vec)
{
    uint8_t r = (uint32_t)(vec.r * 255.0f);
    uint8_t g = (uint32_t)(vec.g * 255.0f);
    uint8_t b = (uint32_t)(vec.b * 255.0f);
    uint8_t a = (uint32_t)(vec.a * 255.0f);

    return (a << 24) | (b << 16) | (g << 8) | r;
}