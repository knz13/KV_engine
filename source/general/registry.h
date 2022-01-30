#pragma once
#include "global.h"
#include "registry_getters.h"
#include "registry_setters.h"
#include "registry_creates.h"




class RegistryCreates;
class RegistryGetters;
class RegistrySetters;
class Registry {
public:

    static RegistryCreates Create();
    static RegistryGetters Get();
    static RegistrySetters Set();
    
    /**
     * Calls the main loop function for each of the windows
     */
    static void MainLoop();

    template<typename T>
    static bool IsFutureDone(std::future<T>& fut);

private:

    static std::unique_ptr<Window> m_MainWindow;
    static std::unordered_map<std::string,std::unique_ptr<Window>> m_SubWindows;


    friend class RegistryCreates;
    friend class RegistryGetters;
    friend class RegistrySetters;
};


