#include <fstream>
#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h" 
#include "inizializzazioni.h"
#include "gestione_curve.h"
#include "gestione_interazioni.h"
#include "geometria.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Gui.h"

#define PI 3.14159265358979323
//Variabili globali -------------------------------------------------------------------
unsigned int programId;
float r = 0.0, g = 0.0, b = 0.0;
float alpha;
int height = 1200, width = 1200;
bool modTg;
Curva player = {}, Derivata = {};//crea una nuova istanza della struttura curva di nome  "curva"
int selected_point = -1;
int i, j;
mat4 Projection;
GLuint MatProj, MatModel, loc_flagP;
float clear_color[3] = { 0.0, 0.0, 1.0 };
float step_t;
float Tens = 0.0, Bias = 0.0, Cont = 0.0;
//----------------------------------------------------------------------------------------

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())   //se glfwInit() d� valore falso, return -1
        return -1;

    //Imposta le propriet� del contesto e del profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Abilita il double buffering

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(height, width, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create the window !" << std::endl;
        glfwTerminate(); //Libera le risorse allcoata da glfwInit
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); //crea il context corrente e lo associa a window. In opengl un rendering context � una macchina astati che memorizza tutte le informazioni necessarie e le risorse per il rendering grafico


    //verifica se la libreria GLAD � riuscita a caricare correttamente tutti i puntatori 
    // alle funzioni OpenGL necessarie.

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load opengl function pointers !" << std::endl;
        glfwTerminate();
        return -1;
    }

    //Registrazione delle funzioni di callback che verranno chiamate quando si verificano
   //  determinati eventi

   //Implementa la chiusura dell'applicazione premendo il tasto Esc  e la modifica del colore dello sfondo con il tasto F2..
    glfwSetKeyCallback(window, key_callback);

    //Per visualizzare le coordinate del mouse che si muove sulla finestra grafica
    glfwSetCursorPosCallback(window, cursor_position_callback);



    //per visualizzare le dimensioni della finestra ridimensionata

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    INIT_SHADER();




    //Registrazione delle funzioni di callback che verranno chiamate quando si verificano
    //  determinati eventi

    //Implementa la chiusura dell'applicazione premendo il tasto Esc  e la modifica del colore dello sfondo con il tasto F2..
    glfwSetKeyCallback(window, key_callback);

    //Per visualizzare le coordinate del mouse che si muove sulla finestra grafica
    glfwSetCursorPosCallback(window, cursor_position_callback);

    //Per visualizzare le coordinate individuate dal tasto sinistro premuto
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    //per visualizzare le dimensioni della finestra ridimensionata

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Projection = ortho(0.0f, float(width), 0.0f, float(height));
    Projection = ortho(-1.0f, 1.0f, -1.0f, 1.0f);


    MatProj = glGetUniformLocation(programId, "Projection");
    MatModel = glGetUniformLocation(programId, "Model");
    loc_flagP = glGetUniformLocation(programId, "flagP");

    /* Loop until the user closes the window */

    //Inizializzazione di un VAO per la curva, con un VBO inizializzato a NULL con un massimo di 100 posizioni, di tipi GL_DYNAMIC_DRAW

    init_player(&player);
    CostruisciHermite(&player);

    INIT_VAO_DYNAMIC_Curva(&player);

    Initialize_IMGUI(window);

    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        glClearColor(clear_color[0], clear_color[1], clear_color[2], 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        my_interface();



        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        player.Model = mat4(1.0);
        //Le coordinate dei vertici della psezzta sono gi� nelle coordinate del mondo, quindi  curva.Model = mat4(1.0);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(player.Model));
        
        glUniform1i(loc_flagP, 0);
        
        CostruisciHermite(&player);
        INIT_VAO_DYNAMIC_Curva(&player);

        glBindVertexArray(player.VAO);
        glDrawArrays(GL_LINE_STRIP, 0, player.vertices.size());  //curva con i vertici in ordine di inserimento

        //Visualizzazione dei vertici come punti
        glBindVertexArray(0);

        


        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Renderizza i dati di disegno di ImGui


        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    close_GUI();

    glDeleteProgram(programId);


    glDeleteBuffers(1, &player.VBO_vertices);
    glDeleteBuffers(1, &player.VBO_colors);
    glDeleteVertexArrays(1, &player.VAO);


    glfwTerminate();


    return 0;
}





