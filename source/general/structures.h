#pragma once

enum WindowFlag {

    None = 0,
    NotResizeable = 1,
    InitiallyMinimized = 2,
    NotFocused = 4,
    OpenGLDebugContext = 8,
    NotDecorated=16,
    FullScreen=32,
    CoreProfile=64,
    FocusOnShow=128
};

enum ShaderType {
    Vertex=0,
    Fragment,
};

struct Transform {
    glm::vec3 position = glm::vec3(0,0,0);
    glm::quat rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);

    void Rotate(glm::vec3 eulerAnglesInDegrees) {
        rotation *= glm::quat(eulerAnglesInDegrees);
    };

    void SetLookDirection(glm::vec3 direction){
        rotation = glm::quatLookAt(direction,glm::vec3(0,1,0));
    }

    glm::vec3 Right(){
        return glm::quat(glm::vec3(0,glm::radians(90.0f),0)) * Forward();
    };

    glm::vec3 Forward() {
        return rotation * glm::vec3(0,0,-1);
    };

    glm::mat4 AsMatrix() {
        return glm::translate(glm::mat4(1.0f),position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f),scale);
    };

};

struct CameraProperties {

    
private:

    glm::mat4 AsProjection() {return glm::mat4(1.0f);};
    glm::mat4 AsView(Transform& transform) {
        return glm::inverse(transform.AsMatrix());
    };
    glm::mat4 AsViewProjection(Transform& transform) {
        return AsProjection() * AsView(transform);
    };

    friend class Camera;

};

struct OrthographicCameraProperties : public CameraProperties {

    float width;
    float height;

private:

    glm::mat4 AsProjection() {
        return glm::ortho(0.0f,width,0.0f,height);
    };

    friend class Camera;

};

struct PerspectiveCameraProperties : public CameraProperties {

    float fov = 65;
    float aspectRatio = 4.0f/3.0f;
    float maxRenderDistance = 1000.0f;
    float minRenderDistance = 0.1f;

private:

    glm::mat4 AsProjection() {
        return glm::perspective(glm::radians(fov),aspectRatio,minRenderDistance,maxRenderDistance);
    };

    friend class Camera;

};





struct ShaderProperties {
    std::string source;
    unsigned int shaderType;


};

struct CameraCreationProperties {
    std::string cameraName = "camera";
    float fov = 45.0f;
    float drawDistance = 100.0f;
    float drawingNearCutoff = 0.1f;
    glm::vec3 initialPos = glm::vec3(0,0,0);
    glm::vec3 initialRotationRadians = glm::vec3(0,0,0);
};

struct WindowCreationProperties {
    int width=640;
    int height=480;
    std::string title="Base Title";
    uint32_t windowFlags = WindowFlag::None;
    int openGLVersionMajor = 4;
    int openGLVersionMinor = 0;
};

class Window;
template<typename T>
struct FunctionSink {
    FunctionSink(std::unordered_map<uint32_t,std::function<T>>& sink);

    uint32_t Connect(std::function<T> windowFunc);
    bool Disconnect(uint32_t handle);

private:

    bool GetNewHandle();

    std::unordered_map<uint32_t,std::function<T>>& m_FuncRef;

    static std::unordered_map<void*,uint32_t> m_HandleCount;

};

template<typename T>
std::unordered_map<void*,uint32_t> FunctionSink<T>::m_HandleCount;

template<typename T>
uint32_t FunctionSink<T>::Connect(std::function<T> windowFunc) {
    uint32_t handle = GetNewHandle();
    m_FuncRef[handle] = std::move(windowFunc);
    return handle;
}

template<typename T>
bool FunctionSink<T>::Disconnect(uint32_t handle) {
    if(m_FuncRef.find(handle) != m_FuncRef.end()){
        m_FuncRef.erase(handle);
        return true;
    }
    return false;
}


template<typename T>
FunctionSink<T>::FunctionSink(std::unordered_map<uint32_t,std::function<T>>& sink) : m_FuncRef(sink){
    if(FunctionSink::m_HandleCount.find((void*)&sink) == FunctionSink::m_HandleCount.end()){
        FunctionSink::m_HandleCount[(void*)&sink] = 0;
    }
}

template<typename T>
bool FunctionSink<T>::GetNewHandle() {

    return FunctionSink::m_HandleCount[(void*)&m_FuncRef]++;
}
