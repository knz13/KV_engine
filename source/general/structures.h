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

struct WindowCreationProperties {
    int width=640;
    int height=480;
    std::string title="Base Title";
    uint32_t windowFlags = WindowFlag::None;
    int openGLVersionMajor = 4;
    int openGLVersionMinor = 0;
};