#pragma once 
#include "../global.h"
#include <functional>


class Camera;
class Drawable;
class Window;

struct WindowEvents {

    WindowEvents(Window& win) : m_Master(win) {};

    FunctionSink<void(Window&)> PreDrawingLoop();
    FunctionSink<void(Window&)> PostDrawingLoop();
    FunctionSink<void(Window&)> Closing();

private:
    Window& m_Master;

};


class Window {

    KV_CLASS

public:

    Window(WindowCreationProperties prop);
    
    ~Window();

    bool IsOpen();
    
    WindowEvents Events();

    
    Window& AddSubWindow(WindowCreationProperties prop);

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
    void DrawingLoop();
    


    WindowCreationProperties m_Properties;
    GLFWwindow* m_ContextPointer=nullptr;
    unsigned int m_DependentID = -1;

    Camera* m_MainCamera = nullptr;
    Window* m_MasterWindow = nullptr;


    std::unordered_map<unsigned int,std::unique_ptr<Window>> m_SubWindows;

    std::map<unsigned int,Drawable*> m_DrawingQueue;
    std::map<uint32_t,std::function<void(Window&)>> m_PreDrawingLoopFuncs;
    std::map<uint32_t,std::function<void(Window&)>> m_PostDrawingLoopFuncs;
    std::map<uint32_t,std::function<void(Window&)>> m_ClosingCallbackFuncs;
    Color m_ClearColor = Color::Black;



    //static members

    static Window* m_MainWindow;


    
    friend struct WindowEvents;


};