#pragma once
#include <iostream>
#include <future>
#include <optional>
#include <memory>
#include <unordered_map>
#include "glm/glm.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "assimp/Importer.hpp"
#include "ryml.hpp"
#include "ryml_std.hpp"
#include "thread_pool.hpp"
#include "general/structures.h"

using namespace std;

#ifdef NDEBUG
    #define DEBUG_LOG(x)
    #define DEBUG_WARN(x)
    #define DEBUG_ERROR(x)
#else
    #define DEBUG_LOG(x) cout << "LOG: " << x << endl <<  " at "<< __LINE__ << endl << " in file: " << __FILE__ << endl
    #define DEBUG_WARN(x) cout << "WARNING: " << x << endl <<  " at "<< __LINE__ << endl << " in file: " << __FILE__ << endl
    #define DEBUG_ERROR(x) cout << "ERROR! -> " << x  << endl <<  " at "<< __LINE__ << endl << " in file: " << __FILE__ << endl; __debugbreak()
#endif

static void ClearGLErrors(){
    while(glGetError() != GL_NO_ERROR){
        ;
    }
}

static void GetGLError(){
    GLenum code = glGetError();
    if(code != GL_NO_ERROR){
        DEBUG_ERROR(gluErrorString(code));
    }
}

#ifdef NDEBUG
    #define GL_CALL(x) x 
#else
    #define GL_CALL(x) ClearGLErrors(); x; GetGLError()
#endif


