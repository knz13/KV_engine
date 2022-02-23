#include "registry_creates.h"
#include "registry.h"
#include "window/window.h"
#include "camera/camera.h"


Window& RegistryCreates::MainWindow(WindowCreationProperties prop) {

    // Clearing the sub windows if any are found
    if(Registry::m_MainWindow){
        DEBUG_WARN("Main window was already defined, returning it...");
        return *Registry::m_MainWindow.get();
    }

    Registry::m_MainWindow = std::make_unique<Window>(prop);
    return *Registry::m_MainWindow.get();
}

Window& RegistryCreates::SubWindow(std::string windowName, WindowCreationProperties prop) {
    prop.windowFlags &= WindowFlag::NotDecorated;

    if(!Registry::m_MainWindow) {
        DEBUG_WARN("Creating subwindow with name '" +windowName +  "' before main one, prefer otherwise!");
        Registry::m_MainWindow = std::make_unique<Window>(WindowCreationProperties());
    }

    if(Registry::m_SubWindows.find(windowName) != Registry::m_SubWindows.end()){
        DEBUG_WARN("SubWindow with name '" + windowName +  "' was already created, returning it.");
        return *Registry::m_SubWindows[windowName].get();
    }

    Registry::m_SubWindows[windowName] = std::make_unique<Window>(prop);

    return *Registry::m_SubWindows[windowName].get();

}



Camera& RegistryCreates::PerspectiveCamera(CameraCreationProperties prop,Window* windowToSetCurrentOn) {
    
    Camera camera;

    camera.m_Fov = prop.fov;
    camera.m_DrawNear = prop.drawingNearCutoff;
    camera.m_DrawDistance = prop.drawDistance;
    camera.m_Position = prop.initialPos;
    camera.m_Rotation = prop.initialRotationRadians;

    if(Registry::m_Cameras.find(prop.cameraName) != Registry::m_Cameras.end()){
        prop.cameraName += "_1";
        int index = 1;
        while(Registry::m_Cameras.find(prop.cameraName) != Registry::m_Cameras.end()){
            prop.cameraName = prop.cameraName.substr(0,prop.cameraName.size()-3) + "_" + std::to_string(index);
            index++;
        }
    }

    Registry::m_Cameras[prop.cameraName] = std::move(camera);

    if(windowToSetCurrentOn){
        windowToSetCurrentOn->SetCamera(Registry::m_Cameras[prop.cameraName]);
    }

    return Registry::m_Cameras[prop.cameraName];

}
