#include "camera.h"
#include "kv.h"



void Camera::SetLookAt(float x, float y, float z) {
    glm::mat4 lookat = glm::lookAt(m_Position,glm::vec3(x,y,z),glm::vec3(0,1,0));
    glm::mat4 modelMat = glm::inverse(lookat);
    modelMat[3] = glm::vec4(0,0,0,1);
    m_Rotation = glm::eulerAngles(glm::quat(modelMat));
}

void Camera::SetDirection(float x, float y, float z) {
    m_Rotation = glm::eulerAngles(glm::quatLookAt(glm::vec3(x,y,z),glm::vec3(0,1,0)));
}

void Camera::Rotate(float pitch, float yaw, float roll) {

    m_Rotation += glm::radians(glm::vec3(pitch,-yaw,roll));
}

void Camera::SetRotation(float pitch, float yaw, float roll) {
    m_Rotation = glm::radians(glm::vec3(pitch,-yaw,roll));
}

void Camera::Move(float x, float y, float z) {
    m_Position.x += x;
    m_Position.y += y;
    m_Position.z += z;
}

void Camera::SetPosition(float x, float y, float z) {
    m_Position = glm::vec3(x,y,z);
}

const glm::vec3& Camera::GetPosition() {
    return m_Position;
}

glm::mat4 Camera::GetViewProjection(const Window& window) {
    return glm::perspective(m_Fov,(float)window.Properties().width/window.Properties().height,m_DrawNear,m_DrawDistance) * glm::inverse(glm::translate(glm::mat4(1.0f),m_Position)* glm::toMat4(glm::quat(m_Rotation)));
}

Camera::Camera() {
    
}

glm::vec3 Camera::GetRotation() {
    return glm::degrees(glm::vec3(m_Rotation.x,-m_Rotation.y,m_Rotation.z));
}

glm::vec4 Camera::GetViewPort() {
    return m_ViewPort;
}
