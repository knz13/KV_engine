#include <iostream>
#include "general/registry.h"
#include "window/window.h"


int main(){
    WindowCreationProperties prop;
    prop.title = "Hey I'm a window!";
    
    Window& win = Registry::Create().MainWindow(prop);


    while(win.IsOpen()){

        
        win.BeginDrawState();




        win.EndDrawState();


    }

    
}