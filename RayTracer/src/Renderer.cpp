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
                pixels[x + y * m_Application->GetWidth()] = RenderPixel(glm::vec2(x, y));
            }
        } 

        m_Image = std::shared_ptr<Magenta::Image>(new Magenta::Image(pixels.data(), glm::vec2(m_Application->GetWidth(), m_Application->GetHeight()), GL_RGBA));
    }
}

uint32_t Renderer::RenderPixel(glm::vec2 pixelPos)
{
    return 0xffff00ff;
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