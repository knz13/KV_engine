#include "registry.h"
#include "registry_creates.h"
#include "registry_getters.h"
#include "registry_setters.h"
#include "window/window.h"
#include <future>


std::unique_ptr<Window> Registry::m_MainWindow;
std::unordered_map<std::string,std::unique_ptr<Window>> Registry::m_SubWindows;

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