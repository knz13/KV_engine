#include <iostream>
#include "kv.h"

int main(){
    WindowCreationProperties prop;
    prop.width = 1280;
    prop.height = 720;
    prop.title = "hello";
    Window win(prop);


    Drawable dr(&win);
    
    std::vector<float> positions = {
      // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0
    };

    std::vector<unsigned int> indices = {
        // front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
    };

    dr.GetVertexArray().CreateVertexBuffer(8)
        .AddAttribute(positions,false)
        .Generate();

    dr.GetVertexArray().CreateIndexBuffer()
        .SetIndices(indices);

    dr.SetPosition(0,0,-10);
    dr.SetShader("res/shaders/base_shader");

    win.Events().MouseButtonEvent().Connect(&dr,[](EventReceiver* rec,Window& win, MouseButtonEventProperties prop){
        static int onOff = 0;
        if(prop.action == GLFW_PRESS){
            Drawable* dr = (Drawable*)rec;
            if(onOff % 2 == 0){
                dr->SetActive();
                std::cout << "active!" << std::endl;
            }
            else {
                dr->SetInactive();
                std::cout << "inactive ;-; !" << std::endl;
            }

            onOff++;
        }
        
        
    });

    


    while(win.IsOpen()){

        win.DrawingLoop();

    }
    


    
}