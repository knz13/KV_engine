#pragma once 
#include "../global.h"



class Window {
public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();
    void BeginDrawState();
    void EndDrawState();

    GLFWwindow* GetContextPointer();

private:
    GLFWwindow* m_ContextPointer=nullptr;
    

};