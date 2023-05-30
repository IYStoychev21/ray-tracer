#include "Camera.h"
#include <iostream>

Camera::Camera(float fov, float nearPlane, float farPlane, Magenta::Application* app)
{
    m_Fov = fov;
    m_NearPlane = nearPlane;
    m_FarPlane = farPlane;
    m_Application = app;

    m_Position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_Direction = glm::vec3(0.0f, 0.0f, -1.0f);
}

Camera::~Camera()
{

}

void Camera::CameraUpdate(float deltaTime)
{
    glm::vec2 mousePos = m_Application->GetInputManager()->GetMousePosition();
    glm::vec2 mouseDelta = mousePos - m_LastMousePositon;
    m_LastMousePositon = mousePos;

    if(!m_Application->GetInputManager()->IsMouseButtonDown(Magenta::MouseButton::Right))
        return;

    glm::vec3 upDirection = {0.0f, 1.0f, 0.0f};
    glm::vec3 rightDirection = glm::cross(m_Direction, upDirection);

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::W))
    {
        m_Position += m_Direction * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::S))
    {
        m_Position -= m_Direction * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::A))
    {
        m_Position -= rightDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::D))
    {
        m_Position += rightDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::Q))
    {
        m_Position += upDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::E))
    {
        m_Position -= upDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }
}