#include "registry_creates.h"
#include "registry.h"
#include "window/window.h"
#include "camera/camera.h"
#include "drawable/drawable.h"
#include "opengl_wrappers/shader.h"






Camera& RegistryCreates::PerspectiveCamera(CameraCreationProperties prop, Window* windowToSetCurrentOn) {
    return Camera::GeneratePerspectiveCamera(prop,windowToSetCurrentOn);
}
