#include "Camera.h"
#include <iostream>

extern glm::vec2 resolution;

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
        m_Position -= m_Direction * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::S))
    {
        m_Position += m_Direction * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::A))
    {
        m_Position += rightDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::D))
    {
        m_Position -= rightDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::Q))
    {
        m_Position -= upDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(m_Application->GetInputManager()->IsKeyDown(Magenta::KeyCode::E))
    {
        m_Position += upDirection * speed * deltaTime;
        m_ShouldReCalculate = true;
    }

    if(mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
    {
        float rotationSpeed = 0.01f;
        float pitch = mouseDelta.y * rotationSpeed;
        float yaw = mouseDelta.x * rotationSpeed;

        glm::quat q = glm::normalize(glm::cross(glm::angleAxis(pitch, rightDirection), glm::angleAxis(yaw, upDirection)));
        m_Direction = glm::rotate(q, m_Direction);

        m_ShouldReCalculate = true;
    }

    if(m_ShouldReCalculate)
    {
        CalculateViewMatrix();
        CalculateRayDirections();
        m_ShouldReCalculate = false;
    }
}

void Camera::CalculateViewMatrix()
{
    m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
    m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
}

void Camera::CalculateProjectionMatrix()
{
    m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), resolution.x / resolution.y, m_NearPlane, m_FarPlane);
    m_InverseProjectionMatrix = glm::inverse(m_ProjectionMatrix);
}

void Camera::CalculateRayDirections()
{
    m_RayDirections.clear();
    m_RayDirections.resize(m_Application->GetWidth() * m_Application->GetHeight());

    for (size_t y = 0; y < m_Application->GetHeight(); y++)
    {
        for (size_t x = 0; x < m_Application->GetWidth(); x++)
        {
            glm::vec2 pixel = glm::vec2(x / (float)m_Application->GetWidth(), y / (float)m_Application->GetHeight());
            pixel = pixel * 2.0f - 1.0f;
            pixel.x *= resolution.x / resolution.y;
            glm::vec4 target = glm::vec4(pixel.x, pixel.y, 1.0f, 1.0f);
            glm::vec3 rayDirection = glm::vec3(m_InverseViewMatrix * glm::vec4(glm::normalize(glm::vec3(target) / target.w), -1.0f));

            m_RayDirections[x + y * m_Application->GetWidth()] = rayDirection;
        }
    }
}

void Camera::Resize()
{
    CalculateViewMatrix();
    CalculateProjectionMatrix();
    CalculateRayDirections();
}