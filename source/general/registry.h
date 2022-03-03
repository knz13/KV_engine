#pragma once
#include "global.h"
#include "registry_getters.h"
#include "registry_setters.h"
#include "registry_creates.h"
#include "registry_deleters.h"

class Shader;
class Drawable;
class Registry {
public:

    static RegistryCreates Create();
    static RegistryGetters Get();
    static RegistrySetters Set();
    static RegistryDeleters Delete();

    /**
     * Calls the main loop function for each of the windows
     */
    static void MainLoop();

    template<typename T>
    static bool IsFutureDone(std::future<T>& fut);

private:
    static float m_DeltaTime;
    
    
    
    
    

    friend class RegistryCreates;
    friend class RegistryGetters;
    friend class RegistrySetters;
    friend class RegistryDeleters;
};


