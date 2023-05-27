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

                pixels[x + y * m_Application->GetWidth()] = RenderPixel(textCoord);
            }
        } 

        m_Image = std::shared_ptr<Magenta::Image>(new Magenta::Image(pixels.data(), glm::vec2(m_Application->GetWidth(), m_Application->GetHeight()), GL_RGBA));
    }
}

uint32_t Renderer::RenderPixel(glm::vec2 coord)
{
    // (bx^2 + by^2)t^2 + (2(axbx + ayby))t + (ax^2 + ay^2 - r^2) = 0
    // a = ray origin
    // b = ray direction
    // r = sphere radius
    // t = distance along ray

    glm::vec3 rayDirection = glm::normalize(glm::vec3(coord.x, coord.y, -1.0f));
    glm::vec3 rayOrigin = glm::vec3(0.0f, 0.0f, 2.0f);

    float radius = 0.5f;

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayDirection, rayOrigin);
    float c = glm::dot(rayOrigin, rayOrigin) - glm::pow(radius, 2.0f);

    float discriminant = glm::pow(b, 2.0f) - 4.0f * a * c;

    if(discriminant >= 0.0f)
        return 0xffff00ff;

    return 0x00000000;
}

bool Renderer::HasResized()
{
    if(m_PrevSize.x != m_Application->GetWidth() || m_PrevSize.y != m_Application->GetHeight())
    {
        m_PrevSize = glm::vec2(m_Application->GetWidth(), m_Application->GetHeight());
        return true;
    }

    return false;
}

void Renderer::RenderUI()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0, 0);

    ImGui::Begin("Viewport");

    ImGui::Image((void*)m_Image->GetTextureID(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    // ImGui::Begin("Settings");
    // ImGui::End();
}