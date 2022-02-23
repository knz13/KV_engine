#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"


class Shader;
class Drawable {

    KV_CLASS

public:
    Drawable();
    ~Drawable();

    void Copy(const Drawable& dr);
    bool SetShader(std::string shaderLocation);
    void SetActive(Window& win);
    void SetActive();
    void SetInactive();

    virtual void Update(float deltaTime);
    
    virtual void Move(float x,float y,float z);
    virtual void SetPosition(float x,float y,float z);

    virtual void Rotate(float x,float y,float z);
    virtual void SetRotation(float x,float y,float z);

    virtual void SetScale(float x,float y,float z);
    virtual void ChangeScale(float x,float y,float z);

    void SetDrawingMode(GLenum mode);

    FunctionSink<void(Drawable&)> Moved();
    FunctionSink<void(Drawable&)> Rotated();
    FunctionSink<void(Drawable&)> Scaled();
    FunctionSink<void(Drawable&,Shader&)> PreDraw();
    FunctionSink<void(Drawable&)> PostDraw();

    VertexArray& GetVertexArray();

private:

    void Draw();
    glm::mat4 GetModelMatrix();
    bool ReadyToDraw();

    glm::vec3 m_Position = glm::vec3(0,0,0);
    glm::vec3 m_Rotation = glm::vec3(0,0,0);
    glm::vec3 m_Scale = glm::vec3(1,1,1);


    GLenum m_DrawingMode = GL_TRIANGLES;
    Window* m_CurrentWindow = nullptr;
    unique_ptr<VertexArray> m_VAO;
    std::string m_ShaderName = "";
    bool m_Active = true;
    unsigned int m_ID;


    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_MovedFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_RotatedFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_ScaledFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&,Shader&)>> m_PreDrawFuncs;
    std::unordered_map<uint32_t,std::function<void(Drawable&)>> m_PostDrawFuncs;


};