#pragma once 
#include "../global.h"
#include <functional>


class Camera;
class Drawable;
class Window {

    KV_CLASS

public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();
    
    FunctionSink<void(Window&)> PreDrawingLoop();
    FunctionSink<void(Window&)> PostDrawingLoop();
    FunctionSink<void(Window&)> Closing();

    

    Camera& GetCurrentCamera();
    void SetClearColor(Color color);
    void SetCamera(Camera& camera);

    const WindowCreationProperties& Properties() const;
    GLFWwindow* GetContextPointer();

protected:

    void AddToDrawingQueue(unsigned int id);
    void RemoveFromDrawingQueue(unsigned int id);

    friend class Drawable;

private:
    void BeginDrawState();
    void EndDrawState();

    
    WindowCreationProperties m_Properties;
    GLFWwindow* m_ContextPointer=nullptr;

    Camera* m_MainCamera = nullptr;
    std::map<unsigned int,Drawable*> m_DrawingQueue;
    std::unordered_map<uint32_t,std::function<void(Window&)>> m_PreDrawingLoopFuncs;
    std::unordered_map<uint32_t,std::function<void(Window&)>> m_PostDrawingLoopFuncs;
    std::unordered_map<uint32_t,std::function<void(Window&)>> m_ClosingCallbackFuncs;
    Color m_ClearColor = Color::Black;
    
    


};