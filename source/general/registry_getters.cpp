#include "registry_getters.h"
#include "kv.h"
#include "registry.h"
#include "window/window.h"
#include <filesystem>

Window* RegistryGetters::MainWindow() {
    if(Window::m_MainWindow){
        return Window::m_MainWindow;
    }
    else {
        DEBUG_WARN("Calling Registry::Get().MainWindow() without any window created. Create window first!");
        return nullptr;
    }
}


float RegistryGetters::DeltaTime() {
    return Registry::m_DeltaTime;
}

bool RegistryGetters::CachedShader(std::string relativeFilePath, Shader& shader) {
    if(Shader::m_LoadedShaders.find(relativeFilePath) != Shader::m_LoadedShaders.end()){
        shader = *Shader::m_LoadedShaders[relativeFilePath].get();
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
    
    Shader::m_LoadedShaders[relativeFilePath] = std::make_unique<Shader>();
    ShaderCreationProperties prop = Shader::m_LoadedShaders[relativeFilePath].get()->CreateNew();
    for(auto& [type,source] : sources){
        prop.AddShader(type,source);
    }

    if(!prop.Generate()){
        Shader::m_LoadedShaders.erase(relativeFilePath);
        return false;
    }

    return true;


}

std::unordered_map<unsigned int,Drawable*>& RegistryGetters::DrawableObjects() {
    return Drawable::m_DrawableObjects;
}


