#pragma once
#include "../global.h"


class Window;
class RegistryGetters {
public:
    Window& MainWindow();

    Window& SubWindow(std::string windowName);

    



};