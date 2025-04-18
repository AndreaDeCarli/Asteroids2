#pragma once 
#include <fstream>
#include "lib.h"
#include "ImGui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"
#include "strutture.h"
#include "gestione_curve.h"
#pragma warning(disable:4996)
#include <math.h>
#include <string>
#include <iostream>

//extern ImVec4 clear_color;
extern int  nClouds;
 extern float clear_color[3];
 extern int Mod, selected_point, score;
 extern bool visualizzaTg, modTg;
 extern Shape curva;
 extern float* t;
 extern float Tens, Bias, Cont;
 extern Actor player;
//bool my_tool_active = true;
void Initialize_IMGUI(GLFWwindow* window){

        IMGUI_CHECKVERSION(); // Verifica la compatibilitÓ della versione di ImGui

        // Create ImGui context
        ImGui::CreateContext(); // Crea un nuovo contesto ImGui

        // Get ImGui IO object to configure options
        ImGuiIO& io = ImGui::GetIO(); // Ottieni l'oggetto IO per configurare opzioni come input, font e stili
        io.FontGlobalScale =2.0f;
        // Set dark theme
        ImGui::StyleColorsDark(); // Imposta il tema scuro per l'interfaccia

        // Initialize ImGui for GLFW and OpenGL 3.3
        ImGui_ImplGlfw_InitForOpenGL(window, true); // Inizializza ImGui per GLFW
        ImGui_ImplOpenGL3_Init("#version 330 core"); // Inizializza ImGui per OpenGL 3.3
}
 
void my_interface( )
{
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame(); // Inizia un nuovo frame per ImGui
        ImGui::NewFrame();

        // Create a window for color settings
        ImGui::Begin("Impostazioni");
        
        ImGui::ColorEdit3("Colore", clear_color);

        ImGui::ProgressBar(player.health);

        std::string str = std::to_string(score);
        const char* chararray = str.c_str();
        ImGui::LabelText(chararray,"Score");

        
        ImGui::End(); // Termina la finestra

        // Render ImGui
        ImGui::Render(); // Renderizza gli elementi dell'interfaccia ImGui

}
 
void close_GUI() {
    ImGui_ImplOpenGL3_Shutdown(); // Chiude l'integrazione con OpenGL
    ImGui_ImplGlfw_Shutdown(); // Chiude l'integrazione con GLFW
    ImGui::DestroyContext(); // Distrugge il contesto ImGui

}
 
 
 