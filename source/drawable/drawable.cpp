#include "drawable.h"
#include "kv.h"


std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> Drawable::MovedFuncs;
std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> Drawable::RotatedFuncs;
std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> Drawable::ScaledFuncs;
std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&,Shader&)>>> Drawable::PreDrawFuncs;
std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> Drawable::PostDrawFuncs;
std::unordered_map<unsigned int,std::map<uint32_t,std::function<void(Drawable&)>>> Drawable::DeletedFuncs;
std::unordered_map<unsigned int,Drawable*> Drawable::m_DrawableObjects;


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

FunctionSink<void(Drawable&)> DrawableEvents::Moved() {
    return FunctionSink(Drawable::MovedFuncs[m_Master.m_ID]);
}

FunctionSink<void(Drawable&)> DrawableEvents::Rotated() {
    return FunctionSink(Drawable::RotatedFuncs[m_Master.m_ID]);
}

FunctionSink<void(Drawable&)> DrawableEvents::Scaled() {
    return FunctionSink(Drawable::ScaledFuncs[m_Master.m_ID]);
}

FunctionSink<void(Drawable&,Shader&)> DrawableEvents::PreDraw() {
    return FunctionSink(Drawable::PreDrawFuncs[m_Master.m_ID]);
}

FunctionSink<void(Drawable&)> DrawableEvents::PostDraw() {
    return FunctionSink(Drawable::PostDrawFuncs[m_Master.m_ID]);
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
    m_CurrentWindow->AddToDrawingQueue(m_ID);
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
    if(!Registry::Get().MainWindow()){
        DEBUG_WARN("Trying to create a new object without any window bound. Create window first!");
    }
    m_VAO = std::unique_ptr<VertexArray>(new VertexArray());
    m_ID = Drawable::CreateDrawableHandle(*this);
    this->SetActive(*Registry::Get().MainWindow());

    //initializing func callbacks

    Drawable::MovedFuncs[m_ID] = std::map<uint32_t,std::function<void(Drawable&)>>();
    Drawable::RotatedFuncs[m_ID] = std::map<uint32_t,std::function<void(Drawable&)>>();
    Drawable::ScaledFuncs[m_ID] = std::map<uint32_t,std::function<void(Drawable&)>>();
    Drawable::PreDrawFuncs[m_ID] = std::map<uint32_t,std::function<void(Drawable&,Shader&)>>();
    Drawable::PostDrawFuncs[m_ID] = std::map<uint32_t,std::function<void(Drawable&)>>();
    Drawable::DeletedFuncs[m_ID] = std::map<uint32_t,std::function<void(Drawable&)>>();

}

Drawable::~Drawable() {

    for(auto[funcHandle,func] : Drawable::DeletedFuncs[m_ID]){
        func(*this);
    }
    SetInactive();
    Drawable::DestroyDrawableHandle(m_ID);

    Drawable::MovedFuncs.erase(m_ID);
    Drawable::RotatedFuncs.erase(m_ID);
    Drawable::ScaledFuncs.erase(m_ID);
    Drawable::PreDrawFuncs.erase(m_ID);
    Drawable::PostDrawFuncs.erase(m_ID);
    Drawable::DeletedFuncs.erase(m_ID);
}

void Drawable::Update(float deltaTime) {
    
}

glm::mat4 Drawable::GetModelMatrix() {
    return glm::translate(glm::mat4(1.0f),m_Position) * glm::toMat4(glm::quat(m_Rotation)) * glm::scale(glm::mat4(1.0f),m_Scale);
}

void Drawable::Draw() {
    m_VAO.get()->Bind();
    if(m_VAO.get()->HasIndexBuffer()){
        GL_CALL(glDrawElements(m_DrawingMode.GetDrawingType(),m_VAO.get()->GetDrawCount(),GL_UNSIGNED_INT,nullptr));
    }
    else {
        GL_CALL(glDrawArrays(m_DrawingMode.GetDrawingType(),0,m_VAO.get()->GetDrawCount()));
    }
}



void Drawable::SetDrawingMode(DrawingMode mode) {
    m_DrawingMode = mode;
}

DrawableEvents Drawable::Events() {
    return DrawableEvents(*this);
}



FunctionSink<void(Drawable&)> DrawableEvents::Deleted() {
    return FunctionSink(Drawable::DeletedFuncs[m_Master.m_ID]);
}

std::map<uint32_t,std::function<void(Drawable&)>> DrawableEvents::MovedCallbacks() {
    return Drawable::MovedFuncs[m_Master.m_ID];
}

std::map<uint32_t,std::function<void(Drawable&)>> DrawableEvents::RotatedCallbacks() {
    return Drawable::RotatedFuncs[m_Master.m_ID];
}

std::map<uint32_t,std::function<void(Drawable&)>> DrawableEvents::ScaledCallbacks() {
    return Drawable::ScaledFuncs[m_Master.m_ID];
}

std::map<uint32_t,std::function<void(Drawable&,Shader&)>> DrawableEvents::PreDrawCallbacks() {
    return Drawable::PreDrawFuncs[m_Master.m_ID];
}

std::map<uint32_t,std::function<void(Drawable&)>> DrawableEvents::PostDrawCallbacks() {
    return Drawable::PostDrawFuncs[m_Master.m_ID];
}

std::map<uint32_t,std::function<void(Drawable&)>> DrawableEvents::DeletedCallbacks() {
    return Drawable::DeletedFuncs[m_Master.m_ID];
}

void Drawable::Copy(const Drawable& dr) {
 
}

void Drawable::DestroyDrawableHandle(unsigned int id) {
    if(m_DrawableObjects.find(id) != m_DrawableObjects.end()){
        m_DrawableObjects.erase(id);
    }
    else{
        DEBUG_WARN("Trying to delete object with id " + std::to_string(id) + " without it being in the drawable objects map.");
    }
}

unsigned int Drawable::CreateDrawableHandle(Drawable& dr) {
    static unsigned int id = 0;

    id++;

    m_DrawableObjects[id] = &dr;

    return id;
}
