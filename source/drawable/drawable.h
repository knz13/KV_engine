#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"

class Drawable {

    KV_CLASS

public:

    
    bool SetShader(std::string shaderLocation);
    
    void Move(float x,float y,float z);
    void SetPosition(float x,float y,float z);

    void Rotate(float x,float y,float z);
    void SetRotation(float x,float y,float z);

    void SetScale(float x,float y,float z);
    void ChangeScale(float x,float y,float z);


    FunctionSink<void(Drawable&)> Moved();
    FunctionSink<void(Drawable&)> Rotated();
    FunctionSink<void(Drawable&)> Scaled();
    FunctionSink<void(Drawable&)> PreDraw();
    FunctionSink<void(Drawable&)> PostDraw();

    VertexArray& GetVertexArray();

private:

    bool ReadyToDraw();

    glm::vec3 m_Position = glm::vec3(0,0,0);
    glm::vec3 m_Rotation = glm::vec3(0,0,0);
    glm::vec3 m_Scale = glm::vec3(1,1,1);

    VertexArray m_VAO;
    std::string m_ShaderName = "";


    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_MovedFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_RotatedFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_ScaledFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_PreDrawFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_PostDrawFuncs;


};