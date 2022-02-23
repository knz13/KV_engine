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

    if(m_Active && m_ShaderName == ""){
        LOG("Created Object has no shader attached, deactivating...");
        m_Active = false;
    }

    Shader shader;
    return m_ShaderName != "" && Registry::Get().CachedShader(m_ShaderName,shader) && m_Active;
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

FunctionSink<void(Drawable&,Shader&)> Drawable::PreDraw() {
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
    return *m_VAO.get();
}

void Drawable::SetActive(Window& win) {
    if(m_CurrentWindow != nullptr){
        m_CurrentWindow->RemoveFromDrawingQueue(m_ID);
    }
    m_CurrentWindow = &win;
    m_Active = true;
}

void Drawable::SetActive(){
    if(m_CurrentWindow != nullptr){
        m_CurrentWindow->AddToDrawingQueue(m_ID);
    }

}

void Drawable::SetInactive() {
    if(m_CurrentWindow != nullptr){
        m_CurrentWindow->RemoveFromDrawingQueue(m_ID);
    }
    
    m_Active = false;
}

Drawable::Drawable() {
    m_VAO = std::unique_ptr<VertexArray>(new VertexArray());
    m_ID = Registry::Create().DrawableObjectHandle(*this);
}

Drawable::~Drawable() {
    Registry::Delete().DestroyDrawableHandle(m_ID);
}

void Drawable::Update(float deltaTime) {
    
}

glm::mat4 Drawable::GetModelMatrix() {
    return glm::translate(glm::mat4(1.0f),m_Position) * glm::toMat4(glm::quat(m_Rotation)) * glm::scale(glm::mat4(1.0f),m_Scale);
}

void Drawable::Draw() {
    m_VAO.get()->Bind();
    if(m_VAO.get()->HasIndexBuffer()){
        GL_CALL(glDrawElements(m_DrawingMode,m_VAO.get()->GetDrawCount(),GL_UNSIGNED_INT,nullptr));
    }
    else {
        GL_CALL(glDrawArrays(m_DrawingMode,0,m_VAO.get()->GetDrawCount()));
    }
}
