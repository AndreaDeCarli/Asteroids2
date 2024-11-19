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

#define PI 3.14159265358979323

extern GLFWwindow* window;
extern float r, g, b;
extern double mousex,mousey;
extern int height, width;
extern bool acc, TURN_LEFT, TURN_RIGHT, shot;
extern Actor player;
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


        case GLFW_KEY_W:  //mentre il tasto W è premuto la variabile di accelerazione rimane true
            if (action == GLFW_PRESS) {
                acc = true;
            }
            else if (action == GLFW_RELEASE) {
                acc = false;
            }
            break;
        case GLFW_KEY_A: //mentre il tasto A è premuto la variabile di rotazione a sinistra rimane true
            if (action == GLFW_PRESS) {
                TURN_LEFT = true;
            }
            else if (action == GLFW_RELEASE) {
                TURN_LEFT = false;
            }
            break;
        case GLFW_KEY_D: //mentre il tasto D è premuto la variabile rotazione a destra rimane true
            if (action == GLFW_PRESS) {
                TURN_RIGHT = true;
            }
            else if (action == GLFW_RELEASE) {
                TURN_RIGHT = false;
            }
            break;
        case GLFW_KEY_SPACE: //quando il tasto SPAZIO viene premuto si attiva la variabile di sparo che verrà poii disattivata nel main
            if (action == GLFW_PRESS) {
                shot = true;
            }
            break;
        default:

            break;
        }


 

    }



void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{

    
}





 
