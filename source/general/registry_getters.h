#pragma once
#include "../global.h"


class Window;
class Shader;
class Drawable;
class RegistryGetters {
public:
    Window* MainWindow();
    Window* SubWindow(std::string windowName);
    std::unordered_map<std::string,std::unique_ptr<Window>>& SubWindows();
    float DeltaTime();
    bool CachedShader(std::string relativeFilePath,Shader& shader);
    std::unordered_map<unsigned int,Drawable*>& DrawableObjects();

};