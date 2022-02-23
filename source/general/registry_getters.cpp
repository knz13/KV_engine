#include "registry_getters.h"
#include "kv.h"
#include "registry.h"
#include "window/window.h"
#include <filesystem>

Window& RegistryGetters::MainWindow() {
    if(Registry::m_MainWindow){
        return *Registry::m_MainWindow.get();
    }
    Registry::m_MainWindow = std::make_unique<Window>(WindowCreationProperties());
    return *Registry::m_MainWindow.get();
}

Window& RegistryGetters::SubWindow(std::string windowName) {
    if(!Registry::m_MainWindow){
        Registry::m_MainWindow = std::make_unique<Window>(WindowCreationProperties());
        DEBUG_WARN("Trying to get subwindow before initializing main one, prefer initializing first!");
    }

    if(Registry::m_SubWindows.find(windowName) != Registry::m_SubWindows.end()){
        return *Registry::m_SubWindows[windowName].get();
    }
    else {
        DEBUG_WARN("Subwindow with name: '" + windowName +"' was not found, creating new window...");
        Registry::m_SubWindows[windowName] = std::make_unique<Window>(WindowCreationProperties{.title=windowName});
        return *Registry::m_SubWindows[windowName].get();
    }
}

std::unordered_map<std::string,std::unique_ptr<Window>>& RegistryGetters::SubWindows() {
    return Registry::m_SubWindows;
}

float RegistryGetters::DeltaTime() {
    return Registry::m_DeltaTime;
}

bool RegistryGetters::CachedShader(std::string relativeFilePath, Shader& shader) {
    if(Registry::m_Shaders.find(relativeFilePath) != Registry::m_Shaders.end()){
        shader = Registry::m_Shaders[relativeFilePath];
        return true;
    }
    
    if(!std::filesystem::exists(relativeFilePath)){
        DEBUG_LOG("Could not load shader at path " + std::filesystem::absolute(relativeFilePath).string());
        return false;
    }
    std::vector<std::pair<ShaderType,std::string>> sources;
    for(auto file : std::filesystem::directory_iterator(relativeFilePath)){
        std::string fileName = file.path().filename().string();

        if(fileName.ends_with("vert")){
            std::string source = LoadFileContents(std::filesystem::absolute(relativeFilePath + "/" + fileName).string());
            sources.push_back(std::make_pair(ShaderType::Vertex,source));
        }
        if(fileName.ends_with("frag")){
            std::string source = LoadFileContents(std::filesystem::absolute(relativeFilePath + "/" + fileName).string());
            sources.push_back(std::make_pair(ShaderType::Fragment,source));
        }
    }

    ShaderCreationProperties prop = shader.CreateNew();
    for(auto& [type,source] : sources){
        prop.AddShader(type,source);
    }

    return prop.Generate();


}

std::unordered_map<unsigned int,Drawable*>& RegistryGetters::DrawableObjects() {
    return Registry::m_DrawableObjects;
}
