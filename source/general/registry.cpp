#include "registry.h"
#include "kv.h"
#include "registry_creates.h"
#include "registry_getters.h"
#include "registry_setters.h"
#include "window/window.h"
#include <future>



float Registry::m_DeltaTime;


template<typename T>
bool Registry::IsFutureDone(std::future<T>& fut) {
    return fut.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

RegistrySetters Registry::Set() {
    return RegistrySetters();
}

RegistryGetters Registry::Get() {
    return RegistryGetters();
}

RegistryCreates Registry::Create() {
    return RegistryCreates();
}
void Registry::MainLoop() {

    static double currentTime = 0;
    static double oldTime = 0;

    if(!Window::m_MainWindow){
        DEBUG_ERROR("Calling main window function without any window defined!");
        return;
    }

    
    while(Window::m_MainWindow->IsOpen()){


        currentTime = glfwGetTime();
        m_DeltaTime = static_cast<float>(currentTime - oldTime);

        oldTime = currentTime;
        
        Window::m_MainWindow->DrawingLoop();
        
    }

}

RegistryDeleters Registry::Delete() {
    return RegistryDeleters();
}
