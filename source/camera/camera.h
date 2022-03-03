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

    glm::vec4 GetViewPort();

private:

    
    
    float m_DrawNear = 0;
    float m_DrawDistance = 100.0f;
    float m_Fov = 45;
    glm::vec4 m_ViewPort = glm::vec4(0,0,1,1);
    glm::vec3 m_Position = glm::vec3(0,0,0);
    glm::vec3 m_Rotation = glm::vec3(0,0,0);

    
    //static members

    static Camera& GeneratePerspectiveCamera(CameraCreationProperties prop,Window* windowToSetCurrentOn=nullptr);

    static std::unordered_map<std::string,Camera> m_Cameras;


};