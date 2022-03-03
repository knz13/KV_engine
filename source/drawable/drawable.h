#pragma once
#include "../global.h"
#include "../opengl_wrappers/vertex_array.h"
#include "drawing_modes.h"

class Shader;
class Drawable;
class DrawableEvents {

    KV_CLASS
public:
    DrawableEvents(Drawable& dr) : m_Master(dr){};

    FunctionSink<void(Drawable&)> Moved();
    FunctionSink<void(Drawable&)> Rotated();
    FunctionSink<void(Drawable&)> Scaled();
    FunctionSink<void(Drawable&,Shader&)> PreDraw();
    FunctionSink<void(Drawable&)> PostDraw();
    FunctionSink<void(Drawable&)> Deleted();


private:
    std::map<uint32_t,std::function<void(Drawable&)>> MovedCallbacks();
    std::map<uint32_t,std::function<void(Drawable&)>> RotatedCallbacks();
    std::map<uint32_t,std::function<void(Drawable&)>> ScaledCallbacks();
    std::map<uint32_t,std::function<void(Drawable&,Shader&)>> PreDrawCallbacks();
    std::map<uint32_t,std::function<void(Drawable&)>> PostDrawCallbacks();
    std::map<uint32_t,std::function<void(Drawable&)>> DeletedCallbacks();


    Drawable& m_Master;

};

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

    void SetDrawingMode(DrawingMode mode);

    DrawableEvents Events();

    VertexArray& GetVertexArray();

private:

    void Draw();
    glm::mat4 GetModelMatrix();
    bool ReadyToDraw();

    glm::vec3 m_Position = glm::vec3(0,0,0);
    glm::vec3 m_Rotation = glm::vec3(0,0,0);
    glm::vec3 m_Scale = glm::vec3(1,1,1);

    


    DrawingMode m_DrawingMode;
    Window* m_CurrentWindow = nullptr;
    unique_ptr<VertexArray> m_VAO;
    std::string m_ShaderName = "";
    bool m_Active = true;
    unsigned int m_ID;


    //static members

    static void DestroyDrawableHandle(unsigned int id);
    static unsigned int CreateDrawableHandle(Drawable& dr);

    static std::unordered_map<unsigned int,Drawable*> m_DrawableObjects;


    static std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> MovedFuncs;
    static std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> RotatedFuncs;
    static std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> ScaledFuncs;
    static std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&,Shader&)>>> PreDrawFuncs;
    static std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> PostDrawFuncs;
    static std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> DeletedFuncs;

    friend class DrawableEvents;
};