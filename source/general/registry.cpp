#include "registry.h"
#include "kv.h"
#include "registry_creates.h"
#include "registry_getters.h"
#include "registry_setters.h"
#include "window/window.h"
#include <future>


std::unordered_map<std::string,Camera> Registry::m_Cameras;
float Registry::m_DeltaTime;
std::unordered_map<std::string,Shader> Registry::m_Shaders;
std::unique_ptr<Window> Registry::m_MainWindow;
std::unordered_map<unsigned int,Drawable*> Registry::m_DrawableObjects;
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
void Registry::MainLoop() {

    static double currentTime = 0;
    static double oldTime = 0;

    if(!m_MainWindow){
        DEBUG_ERROR("Calling main window function without any window defined!");
        return;
    }

    glfwSetWindowCloseCallback(m_MainWindow.get()->m_ContextPointer,[](GLFWwindow* windowPointer){
        for(auto& window : Registry::Get().SubWindows()){
            glfwSetWindowShouldClose(window.second.get()->GetContextPointer(),GL_TRUE);
        }
    });

    
    while(m_MainWindow.get()->IsOpen()){


        currentTime = glfwGetTime();
        m_DeltaTime = static_cast<float>(currentTime - oldTime);

        oldTime = currentTime;
        
        
        glfwMakeContextCurrent(m_MainWindow.get()->m_ContextPointer);
        m_MainWindow.get()->BeginDrawState();
        
        for(auto& func : m_MainWindow.get()->m_PreDrawingLoopFuncs){
            func.second(*m_MainWindow.get());
        }

        for(auto& [handle,objectPointer] : m_MainWindow.get()->m_DrawingQueue){
            
            if(!objectPointer->ReadyToDraw()){
                continue;
            }

            objectPointer->Update(m_DeltaTime);

            Shader& currentObjectShader = m_Shaders[objectPointer->m_ShaderName];
            
            currentObjectShader.Bind();
            currentObjectShader.SetUniformMat4f("MVP",m_MainWindow.get()->GetCurrentCamera().GetViewProjection(*m_MainWindow.get())*objectPointer->GetModelMatrix());

            for (auto& [preDrawFuncHandle,preDrawFunc] : objectPointer->m_PreDrawFuncs){
                preDrawFunc(*objectPointer,currentObjectShader);
            }

            objectPointer->Draw();

            for (auto& [postDrawFuncHandle,postDrawFunc] : objectPointer->m_PostDrawFuncs){
                postDrawFunc(*objectPointer);
            }
        }

        for(auto& func : m_MainWindow.get()->m_PostDrawingLoopFuncs){
            func.second(*m_MainWindow.get());
        }
        m_MainWindow.get()->EndDrawState();

        std::unordered_map<std::string,std::unique_ptr<Window>>::iterator it = m_SubWindows.begin();
        while(it != m_SubWindows.end()){
            if(it->second.get()->IsOpen()){
                glfwMakeContextCurrent(it->second.get()->m_ContextPointer);
                it->second.get()->BeginDrawState();
                for(auto& func : it->second.get()->m_PreDrawingLoopFuncs){
                    func.second(*it->second.get());
                }
                
                for(auto& [handle,objectPointer] : it->second.get()->m_DrawingQueue){
            
                    if(!objectPointer->ReadyToDraw()){
                        continue;
                    }

                    objectPointer->Update(m_DeltaTime);

                    Shader& currentObjectShader = m_Shaders[objectPointer->m_ShaderName];
                    
                    currentObjectShader.Bind();
                    currentObjectShader.SetUniformMat4f("MVP",it->second.get()->GetCurrentCamera().GetViewProjection(*it->second.get())*objectPointer->GetModelMatrix());

                    for (auto& [preDrawFuncHandle,preDrawFunc] : objectPointer->m_PreDrawFuncs){
                        preDrawFunc(*objectPointer,currentObjectShader);
                    }

                    objectPointer->Draw();

                    for (auto& [postDrawFuncHandle,postDrawFunc] : objectPointer->m_PostDrawFuncs){
                        postDrawFunc(*objectPointer);
                    }
                }


                for(auto& func : it->second.get()->m_PostDrawingLoopFuncs){
                    func.second(*it->second.get());
                }
                it->second.get()->EndDrawState();
                it++;
            }
            else {
                it = m_SubWindows.erase(it);
            }
        }    
    }

}

RegistryDeleters Registry::Delete() {
    return RegistryDeleters();
}
