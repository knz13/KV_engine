#pragma once 
#include "../global.h"


enum WindowFlag {

    None = 0
};

class Window {
public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();

    GLFWwindow* GetContextPointer();

    

};