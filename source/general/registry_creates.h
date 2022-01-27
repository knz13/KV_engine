#pragma once
#include "../global.h"

class Window;
class RegistryCreates {

    Window& MainWindow(WindowCreationProperties prop);

    Window& SubWindow(std::string windowName,WindowCreationProperties prop);


};