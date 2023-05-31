#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>

#include "Magenta.h"

class Camera
{
public:
    Camera(float fov, float nearPlane, float farPlane, Magenta::Application* app);
    ~Camera();

    void CameraUpdate(float deltaTime);

    glm::vec3 GetPosition() { return m_Position; }
    std::vector<glm::vec3>& GetRayDirections() { return m_RayDirections; }
    void Resize();

    float speed = 5.0f;

private:
    void CalculateViewMatrix();
    void CalculateProjectionMatrix();
    void CalculateRayDirections();

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
    std::vector<glm::vec3> m_RayDirections;

    bool m_ShouldReCalculate = true;


    Magenta::Application* m_Application;
};