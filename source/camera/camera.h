#pragma once
#include "../global.h"
#include "../object/object.h"

class Camera : public Object {
public:
    Camera();

    void SetCameraProperties(CameraProperties prop);
    glm::mat4 GetViewProjectionMatrix();

    

private:
    CameraProperties m_Properties;


};