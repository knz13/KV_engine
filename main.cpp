#include <iostream>
#include "kv.h"


int main(){
    WindowCreationProperties prop;
    prop.title = "Hey I'm a window!";
    Window& win = Registry::Create().MainWindow(prop);
    
    

    Shader shader;

    shader.CreateNew()
        .AddShader(ShaderType::Vertex,R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        void main()
        {
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        }
    )")
        .AddShader(ShaderType::Fragment,R"(
        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);
        } 
    )")
        .Generate();

    VertexArray va; 
    std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    std::vector<unsigned int> indices {
        0, 1, 3,   
        1, 2, 3 
    };

    va.CreateVertexBuffer(4)
        .AddAttribute(vertices,false)
        .Generate();

    va.CreateIndexBuffer()
        .SetIndices(indices);

    win.DrawingLoop().Connect([&](Window& window){
        shader.Bind();
        va.Bind();
        if(!va.HasIndexBuffer()){
            GL_CALL(glDrawArrays(GL_TRIANGLES,0,3));
        }
        else{
            GL_CALL(glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0));
        }

    });

    Registry::MainLoop();
    
    
}