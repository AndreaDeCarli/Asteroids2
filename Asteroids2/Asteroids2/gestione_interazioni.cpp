#pragma once
#include <iostream>
#include <fstream>

#include "lib.h"
#include "strutture.h"
#include "gestione_interazioni.h"
#include "inizializzazioni.h"
#include "gestione_curve.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Gui.h"

extern GLFWwindow* window;
extern Curva curva, Derivata;
extern float Tens, Bias, Cont;
extern float r, g, b;
extern double mousex,mousey;
extern int height, width;
extern int Mod;
extern bool modTg;
extern int selected_point;
extern float* t;
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    
}
 
    void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {

        // Se il tasto ESCAPE è premuto, chiude la finestra
        switch (key) {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                //Imposta a True il flag booleano di chiusura della finestr
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        default:

            break;
        }


 

    }



void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{

    

    
}





 
