#pragma once 

#include <glm/glm.hpp>
#include "Magenta.h"

class Camera
{
public:
    Camera(float fov, float nearPlane, float farPlane, Magenta::Application* app);
    ~Camera();

    void CameraUpdate(float deltaTime);

    glm::vec3 GetPosition() { return m_Position; }

    float speed = 2.0f;
private:
    float m_Fov = 45.0f;
    float m_NearPlane = 0.1f;
    float m_FarPlane = 100.0f;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_InverseProjectionMatrix;
    glm::mat4 m_InverseViewMatrix;

    glm::vec2 m_LastMousePositon = {0.0f, 0.0f};

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_Direction = {0.0f, 0.0f, 0.0f};

    bool m_ShouldReCalculate = true;


    Magenta::Application* m_Application;
};