#pragma once
#include "../global.h"

class Window;
class Camera {

    KV_CLASS
    
public: 
    Camera();

    void SetLookAt(float x,float y,float z);
    void SetDirection(float x,float y,float z);

    void Rotate(float pitch,float yaw,float roll);
    void SetRotation(float pitch,float yaw,float roll);

    void Move(float x,float y,float z);
    void SetPosition(float x,float y,float z);
    
    glm::vec3 GetRotation();
    const glm::vec3& GetPosition();
    glm::mat4 GetViewProjection(const Window& window);


private:

    
    
    float m_DrawNear = 0;
    float m_DrawDistance = 100.0f;
    float m_Fov = 45;
    glm::vec3 m_Position;
    glm::vec3 m_Rotation;

    
};