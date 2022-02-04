#include "camera.h"



void Camera::SetCameraProperties(CameraProperties prop) {
    
}

glm::mat4 Camera::GetViewProjectionMatrix() {
    return m_Properties.AsViewProjection(this->transform);
}

Camera::Camera() {
    PerspectiveCameraProperties prop;
    m_Properties = prop;
    transform.SetLookDirection(glm::vec3(0,0,-1));
}
