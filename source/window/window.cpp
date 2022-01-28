#include "window.h"

Window::Window(WindowCreationProperties prop) {
    
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

    

}

Window::~Window() {
    
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

    GL_CALL(glClearColor(0.0f,0.0f,0.0f,0.0f));

    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}
