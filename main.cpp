#include <iostream>
#include "kv.h"

int main(){
    WindowCreationProperties prop;
    prop.width = 1280;
    prop.height = 720;
    prop.title = "hello";
    Window win(prop);


    Camera& camera = win.GetCurrentCamera();
    
    std::vector<std::unique_ptr<Drawable>> modelsAll;
    for(int i = 0;i< 20 ; i++){
        for(int y = 0;y< 20; y++){
            std::unique_ptr<Drawable> dr = std::make_unique<Drawable>();
            LoadingModelProperties prop;
            prop.initializationFunc = [=](Drawable& model){
                    model.SetPosition(-10 + y*3,i*3,-10);
                    model.SetShader("res/shaders/base_shader");
                };
            if(ModelLoader::LoadModel("res/models/polyfit_sfm_filtered_normals_ransac_3.obj",*dr.get(),prop)){
                modelsAll.push_back(std::move(dr));
            }
        }
    }


    




    win.Events().PreDrawingLoop().Connect([&](Window& win){
        
        //std::cout << camera.GetRotation().x << " " << camera.GetRotation().y <<" " << camera.GetRotation().z << endl;
        if(glfwGetKey(win.GetContextPointer(),GLFW_KEY_A)){
            camera.Rotate(0,-60*Registry::Get().DeltaTime(),0);
        }
        if(glfwGetKey(win.GetContextPointer(),GLFW_KEY_D)){
            camera.Rotate(0,60*Registry::Get().DeltaTime(),0);
        }
        if(glfwGetKey(win.GetContextPointer(),GLFW_KEY_W)){
            camera.Rotate(60*Registry::Get().DeltaTime(),0,0);
        }
        if(glfwGetKey(win.GetContextPointer(),GLFW_KEY_S)){
            camera.Rotate(-60*Registry::Get().DeltaTime(),0,0);
        }   
    });


    Registry::MainLoop();  
}