#include <iostream>
#include "general/registry.h"
#include "window/window.h"
#include "opengl_wrappers/vertex_array.h"

int main(){
    WindowCreationProperties prop;
    prop.title = "Hey I'm a window!";
    Window& win = Registry::Create().MainWindow(prop);
    win.SetClearColor(Color::Blue);
    

    

    Registry::MainLoop();
    
    
}