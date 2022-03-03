#pragma once
#include "../global.h"


class Window;
class Shader;
class Drawable;
class RegistryGetters {
public:
    Window* MainWindow();
    float DeltaTime();
    bool CachedShader(std::string relativeFilePath,Shader& shader);
    std::unordered_map<unsigned int,Drawable*>& DrawableObjects();

};