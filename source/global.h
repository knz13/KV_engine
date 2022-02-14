#pragma once
#include <iostream>
#include <future>
#include <optional>
#include <memory>
#include <unordered_map>
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/quaternion.hpp"
#include "../vendor/glm/glm/gtx/quaternion.hpp"
#define GLEW_STATIC
#include "../vendor/glew/include/GL/glew.h"
#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/misc/cpp/imgui_stdlib.h"
#include "../vendor/thread_pool/thread_pool.hpp"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "general/structures.h"
#include "general/color.h"

using namespace std;


#define GL_SIZEOF(x) TestSize(x)


static unsigned int TestSize(unsigned int dataType) {
    switch (dataType) {
    case GL_FLOAT:
        return sizeof(float);
    case GL_UNSIGNED_INT:
        return sizeof(unsigned int);
    case GL_UNSIGNED_BYTE:
        return sizeof(unsigned char);
    case GL_BYTE:
        return sizeof(char);
    case GL_INT:
        return sizeof(int);
    default:
        return 0;
    }
}


#define LOG(x) cout << "LOG: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl


#ifdef NDEBUG
    #define DEBUG_LOG(x)
    #define DEBUG_WARN(x)
    #define DEBUG_ERROR(x)
#else
    #define DEBUG_LOG(x) cout << "LOG: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
    #define DEBUG_WARN(x) cout << "WARNING: " << x << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl
    #define DEBUG_ERROR(x) cout << "ERROR! -> " << x  << endl <<  "At line: "<< __LINE__ << endl << "In file: " << __FILE__ << endl; __debugbreak()
#endif

static void ClearGLErrors(){
    while(glGetError() != GL_NO_ERROR){
        ;
    }
}

static bool GetGLError(int line,std::string file){
    GLenum code = glGetError();
    if(code != GL_NO_ERROR){
        cout << "OpenGL error '" << gluErrorString(code) << "' \nAt line: " << line << " \nIn file: " << file << endl;
        return true;
    }
    return false;
}

#ifdef NDEBUG
    #define GL_CALL(x) x 
#else
    #define GL_CALL(x) ClearGLErrors(); x; if(GetGLError(__LINE__,__FILE__)) {__debugbreak();}
#endif


