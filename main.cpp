#include <iostream>
#include "kv.h"
#include "imgui.h"

int main(){

    Window::WindowCreationEvent().Connect([](Window& win){
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(win.GetContextPointer(), true);
        ImGui_ImplOpenGL3_Init("#version 430 core");
    });


    WindowCreationProperties prop;
    prop.width = 1280;
    prop.height = 720;
    prop.title = "window";
    Window win(prop);

    win.Events().PreDrawingLoopEvent().Connect([](Window& win){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hey!");


        ImGui::End();
    });

    win.Events().PostDrawingLoopEvent().Connect([](Window& win){
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Render();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    });


    Drawable dr(&win);

    ModelLoader::LoadModel("res/models/polyfit_sfm_filtered_normals_ransac_3.obj",dr);
    dr.SetPosition(0,0,-10);
    dr.SetShader("res/shaders/base_shader");

    win.Events().MouseButtonEvent().Connect(&dr,[](EventReceiver* rec,Window& win, MouseButtonEventProperties prop){
        static int onOff = 0;
        Drawable* dr = (Drawable*)rec;

        if(prop.action == GLFW_PRESS){

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

    win.Events().KeyEvent().Connect(&dr,[](EventReceiver* rec,Window& win,KeyEventProperties prop){
        Drawable* dr = (Drawable*)rec;
        if(prop.action == GLFW_PRESS){

            if(prop.key == GLFW_KEY_A){
                LineMode mode;
                dr->SetDrawingMode(mode);
            }
            if(prop.key == GLFW_KEY_D){
                TriangleMode mode;
                dr->SetDrawingMode(mode);
            }
            if(prop.key == GLFW_KEY_W){
                PointsMode mode;
                dr->SetDrawingMode(mode);
            }
            
        }
    });

    
    
    


    while(win.IsOpen()){



        win.DrawingLoop();

    }
    


    
}