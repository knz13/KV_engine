#pragma once 
#include "../global.h"
#include <functional>

class Window {
public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();
    void BeginDrawState();
    void EndDrawState();

    void SetDrawingLoop(std::function<void(Window&)> windowFunc);
    void SetClosingCallback(std::function<void(Window&)>windowFunc);

    void SetClearColor(Color color);

    GLFWwindow* GetContextPointer();

private:
    GLFWwindow* m_ContextPointer=nullptr;
    std::function<void(Window&)> m_DrawingLoop = [](Window& win){};
    std::function<void(Window&)> m_ClosingCallback = [](Window& win){};
    Color m_ClearColor = Color::Black;
    

    friend class Registry;

};