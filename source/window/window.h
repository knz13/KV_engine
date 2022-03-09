#pragma once 
#include "../global.h"
#include <functional>


class Camera;
class Drawable;
class Window;
class VertexArray;
class Shader;
struct WindowEvents {

    WindowEvents(Window& win) : m_Master(win) {};


    FunctionSink<void(Window&,MouseScrollEventProperties)> MouseScrollEvent();
    FunctionSink<void(Window&,MouseButtonEventProperties)> MouseButtonEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseEnteredWindowEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseLeftWindowEvent();
    FunctionSink<void(Window&,KeyEventProperties)> KeyEvent();
    FunctionSink<void(Window&,MouseEventProperties)> MouseEvent();
    FunctionSink<void(Window&)> PreDrawingLoopEvent();
    FunctionSink<void(Window&)> PostDrawingLoopEvent();
    FunctionSink<void(Window&)> ClosingEvent();
    FunctionSink<void(Window&,WindowResizedEventProperties)> ResizedEvent();

private:
    Window& m_Master;

};

class WindowCreators {
public:
    WindowCreators(Window& wid) : m_Master(wid){}

    VertexArray& NewVertexArray();
    Shader& CachedShader(std::string shaderRelativePath,bool* loadResult);
    

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

    WindowCreators Create();
    

    Camera& GetCurrentCamera();
    void SetClearColor(Color color);
    void SetCamera(Camera& camera);

    const WindowCreationProperties& Properties() const;
    GLFWwindow* GetContextPointer();

    static Window* GetWindow(GLFWwindow* win);
    void DrawingLoop();

    
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
    


    float m_DeltaTime = 0;


    std::vector<std::unique_ptr<VertexArray>> m_CreatedVertexArrays;
    std::map<std::string,std::unique_ptr<Shader>> m_CreatedShaders;
    std::map<unsigned int,Drawable*> m_DrawingQueue;

    
    EventLauncher<void(Window&,WindowResizedEventProperties)> m_WindowResizedEventFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseMovedFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseEnteredWindowFuncs;
    EventLauncher<void(Window&,MouseEventProperties)> m_MouseLeftWindowFuncs;
    EventLauncher<void(Window&,MouseButtonEventProperties)> m_MouseButtonFuncs;
    EventLauncher<void(Window&,MouseScrollEventProperties)> m_MouseScrollFuncs;
    EventLauncher<void(Window&,KeyEventProperties)> m_KeyEventFuncs;
    EventLauncher<void(Window&)> m_PreDrawingLoopFuncs;
    EventLauncher<void(Window&)> m_PostDrawingLoopFuncs;
    EventLauncher<void(Window&)> m_ClosingCallbackFuncs;
    Color m_ClearColor = Color::Black;



    //static members



    static std::map<GLFWwindow*,Window*> m_CurrentWindows;

    friend struct WindowCreators;
    friend struct WindowEvents;


};