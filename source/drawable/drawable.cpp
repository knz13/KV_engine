#include "drawable.h"
#include "kv.h"



bool Drawable::SetShader(std::string shaderLocation) {
    Shader shader;
    if(Registry::Get().CachedShader(shaderLocation,shader)){
        m_ShaderName = shaderLocation;
        return true;
    }
    else {
        return false;
    }
}

bool Drawable::ReadyToDraw() {
    Shader shader;
    return m_ShaderName != "" && Registry::Get().CachedShader(m_ShaderName,shader);
}

FunctionSink<void(Drawable&)> Drawable::Moved() {
    return FunctionSink(m_MovedFuncs);
}

FunctionSink<void(Drawable&)> Drawable::Rotated() {
    return FunctionSink(m_RotatedFuncs);
}

FunctionSink<void(Drawable&)> Drawable::Scaled() {
    return FunctionSink(m_ScaledFuncs);
}

FunctionSink<void(Drawable&)> Drawable::PreDraw() {
    return FunctionSink(m_PreDrawFuncs);
}

FunctionSink<void(Drawable&)> Drawable::PostDraw() {
    return FunctionSink(m_PostDrawFuncs);
}

void Drawable::Move(float x, float y, float z) {
    m_Position += glm::vec3(x,y,z);
}

void Drawable::SetPosition(float x, float y, float z) {
    m_Position = glm::vec3(x,y,z);
}

void Drawable::Rotate(float x, float y, float z) {
    m_Rotation += glm::vec3(x,y,z);
}

void Drawable::SetRotation(float x, float y, float z) {
    m_Rotation = glm::vec3(x,y,z);
}

void Drawable::SetScale(float x, float y, float z) {
    m_Scale = glm::vec3(x,y,z);
}

void Drawable::ChangeScale(float x, float y, float z) {
    m_Scale += glm::vec3(x,y,z);
}

VertexArray& Drawable::GetVertexArray() {
    return m_VAO;
}
