#include "window.h"
#include "kv.h"

Window* Window::m_MainWindow = nullptr;

Window::Window(WindowCreationProperties prop) : m_Properties(prop) {
    
    if(Window::m_MainWindow != nullptr){
        DEBUG_WARN("Creating a new Window with a main window already bound, deleting current window.");
        Window::m_MainWindow->~Window();
        Window::m_MainWindow = nullptr;
    }

    if(glfwInit() != GLFW_TRUE){
        DEBUG_ERROR("GLFW was not initiated!");
        return;
    }

    if(prop.windowFlags != WindowFlag::None){
        if(prop.windowFlags & WindowFlag::OpenGLDebugContext){
            glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GLFW_TRUE);
        }
        if(prop.windowFlags & WindowFlag::InitiallyMinimized){
            glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::NotDecorated){
            glfwWindowHint(GLFW_DECORATED,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::NotResizeable){
            glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::NotFocused){
            glfwWindowHint(GLFW_FOCUSED,GLFW_FALSE);
        }
        if(prop.windowFlags & WindowFlag::FocusOnShow){
            glfwWindowHint(GLFW_FOCUS_ON_SHOW,GLFW_TRUE);
        }
        
        if((prop.openGLVersionMajor == 3 && prop.openGLVersionMinor > 2) || prop.openGLVersionMajor > 3){
            if(prop.windowFlags & WindowFlag::CoreProfile){
                glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
            }
            else {
                glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
            }
        }
    }

    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,prop.openGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,prop.openGLVersionMinor);

    if(prop.windowFlags & WindowFlag::FullScreen){
        this->m_ContextPointer = glfwCreateWindow(prop.width,prop.height,prop.title.c_str(),glfwGetPrimaryMonitor(),NULL);
    }
    else {
        this->m_ContextPointer = glfwCreateWindow(prop.width,prop.height,prop.title.c_str(),NULL,NULL);
    }
    
    glfwMakeContextCurrent(this->m_ContextPointer);

    glewExperimental = true;
    if(glewInit() != GLEW_OK){
        DEBUG_ERROR("Glew wasn't initiated!");
        return;
    }

    CameraCreationProperties camProp;
    Registry::Create().PerspectiveCamera(camProp,this);

    GL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));

    Window::m_MainWindow = this;

}

Window::~Window() {

    m_SubWindows.clear();

    for(auto& func : m_ClosingCallbackFuncs){
        func.second(*this);
    }

    for(auto& [handle,obj] : m_DrawingQueue){
       obj->~Drawable();
    }

    glfwDestroyWindow(m_ContextPointer);
}

bool Window::IsOpen() {
    return !glfwWindowShouldClose(m_ContextPointer);
}

GLFWwindow* Window::GetContextPointer() {
    return m_ContextPointer;
}

void Window::EndDrawState() {
    glfwSwapBuffers(m_ContextPointer);
}

void Window::BeginDrawState() {
    glfwPollEvents();
    glfwGetWindowSize(m_ContextPointer, &m_Properties.width, &m_Properties.height);
    
    glm::vec4 viewport = GetCurrentCamera().GetViewPort();
    GL_CALL(glViewport(viewport.x*m_Properties.width,viewport.y*m_Properties.height,viewport.z*m_Properties.width,viewport.w*m_Properties.height));

    glm::vec3 color = m_ClearColor.Normalized();
    GL_CALL(glClearColor(color.x,color.y,color.z,1.0f));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}



void Window::SetClearColor(Color color) {
    m_ClearColor = color;
}

const WindowCreationProperties& Window::Properties() const {
    return m_Properties;
}

FunctionSink<void(Window&)> WindowEvents::PostDrawingLoop() {
    return FunctionSink<void(Window&)>(m_Master.m_PostDrawingLoopFuncs);
}

FunctionSink<void(Window&)> WindowEvents::PreDrawingLoop() {
    return FunctionSink<void(Window&)>(m_Master.m_PreDrawingLoopFuncs);
}

FunctionSink<void(Window&)> WindowEvents::Closing() {
    return FunctionSink<void(Window&)>(m_Master.m_ClosingCallbackFuncs);
}
void Window::SetCamera(Camera& camera) {
    m_MainCamera = &camera;
}

Camera& Window::GetCurrentCamera() {
    return *m_MainCamera;
}

void Window::AddToDrawingQueue(unsigned int id) {
    m_DrawingQueue[id] = Registry::Get().DrawableObjects()[id];
}

void Window::RemoveFromDrawingQueue(unsigned int id) {
    if(m_DrawingQueue.find(id) != m_DrawingQueue.end()){
        m_DrawingQueue.erase(id);
    }
}

WindowEvents Window::Events() {
    return WindowEvents(*this);
}

void Window::DrawingLoop() {

    glfwMakeContextCurrent(m_ContextPointer);
    BeginDrawState();
    
    

    for(auto& func : m_PreDrawingLoopFuncs){
        func.second(*this);
    }


    //drawing objects 
    for(auto& [handle,objectPointer] : m_DrawingQueue){
            
        if(!objectPointer->ReadyToDraw()){
            continue;
        }

        objectPointer->Update(Registry::Get().DeltaTime());

        Shader& currentObjectShader = *Shader::m_LoadedShaders[objectPointer->m_ShaderName].get();
        
        currentObjectShader.Bind();
        currentObjectShader.SetUniformMat4f("MVP", GetCurrentCamera().GetViewProjection(*this)*objectPointer->GetModelMatrix());

        for (auto& [preDrawFuncHandle,preDrawFunc] : objectPointer->Events().PreDrawCallbacks()){
            preDrawFunc(*objectPointer,currentObjectShader);
        }

        objectPointer->Draw();

        for (auto& [postDrawFuncHandle,postDrawFunc] : objectPointer->Events().PostDrawCallbacks()){
            postDrawFunc(*objectPointer);
        }
    }

    for(auto& func : m_PostDrawingLoopFuncs){
        func.second(*this);
    }

    EndDrawState();

    auto it = m_SubWindows.begin();

    while(it != m_SubWindows.end()){
        if(it->second.get()->IsOpen()){
                it->second.get()->DrawingLoop();
                it++;
            }
            else {
                it = m_SubWindows.erase(it);
            }


    }

    //drawing for subwindows

}

Window& Window::AddSubWindow(WindowCreationProperties prop) {
    static unsigned int id = -1;
    id++;

    m_SubWindows[id] = std::unique_ptr<Window>(new Window(prop));
    

    return *m_SubWindows[id].get();
}
