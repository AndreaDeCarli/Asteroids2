#include <fstream>
#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h" 
#include "inizializzazioni.h"
#include "gestione_curve.h"
#include "gestione_interazioni.h"
#include "geometria.h"
#include "Utilities.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Gui.h"

#define PI 3.14159265358979323

#define INITIAL_ASTEROID_R 1.2
#define REGULAR_ASTEROID_R 0.5
#define SMALL_ASTEROID_R 0.1

//Variabili globali -------------------------------------------------------------------
unsigned int fgShaders, bgShaders;
float r = 0.0, g = 0.0, b = 0.0;
float alpha;
int height = 1000, width = 1000, collision_timer = 0;
Actor player = {}, background = {}, projectile = {};
Actor* Asteroids[10] = {};
int i, j;
mat4 Projection;
GLuint MatProj, MatModel, loc_flagP, GameColor, vec_resS, loc_time;
float clear_color[3] = { 1.0, 1.0, 1.0 };
float step_t, scale_factor;
bool acc = false, TURN_LEFT = false, TURN_RIGHT = false, can_collide = true, shot = false;
const double fpsLimit = 1.0 / 60.0;
double lastUpdateTime = 0;  // number of seconds since the last loop
double lastFrameTime = 0;   // number of seconds since the last frame

//----------------------------------------------------------------------------------------

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())   //se glfwInit() dà valore falso, return -1
        return -1;

    //Imposta le proprietà del contesto e del profile
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
    glfwMakeContextCurrent(window); //crea il context corrente e lo associa a window. In opengl un rendering context è una macchina astati che memorizza tutte le informazioni necessarie e le risorse per il rendering grafico


    //verifica se la libreria GLAD è riuscita a caricare correttamente tutti i puntatori 
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


    MatProj = glGetUniformLocation(fgShaders, "Projection");
    MatModel = glGetUniformLocation(fgShaders, "Model");
    GameColor = glGetUniformLocation(fgShaders, "GameColor");
    loc_flagP = glGetUniformLocation(fgShaders, "flagP");

    MatProj = glGetUniformLocation(bgShaders, "Projection");
    MatModel = glGetUniformLocation(bgShaders, "Model");
    GameColor = glGetUniformLocation(bgShaders, "GameColor");
    loc_flagP = glGetUniformLocation(bgShaders, "flagP");
    loc_time = glGetUniformLocation(bgShaders, "time");
    vec_resS = glGetUniformLocation(bgShaders, "resolution");

    /* Loop until the user closes the window */

    //Inizializzazione di un VAO per la curva, con un VBO inizializzato a NULL con un massimo di 100 posizioni, di tipi GL_DYNAMIC_DRAW

    scale_factor = 0.05;

    init_player_actor(&player);
    init_background_actor(&background);
    init_projectile_actor(&projectile);

    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        Asteroids[i] = init_asteroid(INITIAL_ASTEROID_R, i);
        INIT_VAO_DYNAMIC_Curva(Asteroids[i]->shape);
    }

    INIT_VAO_DYNAMIC_Curva(player.shape);
    INIT_VAO_DYNAMIC_Curva(background.shape);
    INIT_VAO_DYNAMIC_Curva(projectile.shape);

    Initialize_IMGUI(window);

    while (!glfwWindowShouldClose(window))
    {
        

        double now = glfwGetTime();
        double deltaTime = now - lastUpdateTime;

        glfwPollEvents();

        

        
        
        
        if ((now - lastFrameTime) >= fpsLimit)
        {

            if (acc)
                player.velocity += 0.005;
            else if (player.velocity > 0) 
                player.velocity -= 0.004;
            if (player.velocity < 0) 
                player.velocity = 0;

            if (TURN_LEFT) 
                player.direction += 2 * PI * 0.01;
            if (TURN_RIGHT) 
                player.direction -= 2 * PI * 0.01;

            if (can_collide == false) {
                collision_timer += 1;
                if (collision_timer >= 30) {
                    can_collide = true;
                    collision_timer = 0;
                }

            }

            player.position.x += (player.velocity * cos(player.direction));
            player.position.y += (player.velocity * sin(player.direction));


            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT);
            my_interface();

            //drawing background

            glUseProgram(bgShaders);
            
            background.shape->Model = mat4(1.0);
            background.shape->Model = scale(background.shape->Model, vec3(float(width) * 2.0, float(height) * 2.0, 1.0));

            glUniform4fv(GameColor,1, value_ptr(vec4(clear_color[0], clear_color[1], clear_color[2], 0.0)));
            glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
            glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(background.shape->Model));
            glUniform2fv(vec_resS, 1, value_ptr(vec2((float)height, (float)width)));
            glUniform1f(loc_time, now);

            glBindVertexArray(background.shape->VAO);
            glDrawArrays(background.shape->render, 0, background.shape->nv);

            //drawing player
            glUseProgram(fgShaders);

            

            glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
            player.shape->Model = mat4(1.0);
            player.shape->Model = scale(player.shape->Model, vec3(scale_factor, scale_factor, 1.0));
            player.shape->Model = translate(player.shape->Model, vec3(player.position.x, player.position.y, 0.0));
            updateBB(player.shape);
            player.shape->Model = rotate(player.shape->Model, player.direction-(float)PI/2 ,vec3(0.0, 0.0, 1.0));
            glUniform4fv(GameColor, 1, value_ptr(vec4(clear_color[0], clear_color[1], clear_color[2], 0.0)));
            glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(player.shape->Model));
            glUniform1i(loc_flagP, 0);
            glLineWidth(2.0);

            glBindVertexArray(player.shape->VAO);
            glDrawArrays(player.shape->render, 0, player.shape->vertices.size());
            glBindVertexArray(0);

            //pacman effect to always see the player
            pacmanEffect(&player,20.0);

            //Drawing projectile

            if (shot) {
                projectile.position.x += (projectile.velocity * cos(projectile.direction));
                projectile.position.y += (projectile.velocity * sin(projectile.direction));

                glUseProgram(fgShaders);

                glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
                projectile.shape->Model = mat4(1.0);
                projectile.shape->Model = scale(projectile.shape->Model, vec3(scale_factor, scale_factor, 1.0));
                projectile.shape->Model = translate(projectile.shape->Model, vec3(projectile.position.x, projectile.position.y, 0.0));
                updateBB(projectile.shape);
                projectile.shape->Model = rotate(projectile.shape->Model, projectile.direction - (float)PI / 2, vec3(0.0, 0.0, 1.0));
                glUniform4fv(GameColor, 1, value_ptr(vec4(clear_color[0], clear_color[1], clear_color[2], 0.0)));
                glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(projectile.shape->Model));
                glUniform1i(loc_flagP, 0);

                glBindVertexArray(projectile.shape->VAO);
                glDrawArrays(projectile.shape->render, 0, projectile.shape->vertices.size());
                glBindVertexArray(0);

                if (outsideBoundary(&projectile, 20.0)) {
                    shot = false;
                }

            }
            else {

                projectile.position.x = player.position.x;
                projectile.position.y = player.position.y;
                projectile.direction = player.direction;
                projectile.shape->Model = mat4(1.0);
                projectile.shape->Model = scale(projectile.shape->Model, vec3(scale_factor, scale_factor, 1.0));
                projectile.shape->Model = translate(projectile.shape->Model, vec3(projectile.position.x, projectile.position.y, 0.0));
                updateBB(projectile.shape);
            }

            //drawing Asteroids

            for (int i = 0; i < 10; i++){

                Asteroids[i]->position.x += (Asteroids[i]->velocity * cos(Asteroids[i]->direction));
                Asteroids[i]->position.y += (Asteroids[i]->velocity * sin(Asteroids[i]->direction));

                glUseProgram(fgShaders);

                glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
                Asteroids[i]->shape->Model = mat4(1.0);
                Asteroids[i]->shape->Model = scale(Asteroids[i]->shape->Model, vec3(scale_factor, scale_factor, 1.0));
                Asteroids[i]->shape->Model = translate(Asteroids[i]->shape->Model, vec3(Asteroids[i]->position.x, Asteroids[i]->position.y, 0.0));
                updateBB(Asteroids[i]->shape);

                Asteroids[i]->shape->Model = rotate(Asteroids[i]->shape->Model, Asteroids[i]->direction, vec3(0.0, 0.0, 1.0));
                
                if (checkCollision(player.shape, Asteroids[i]->shape) && can_collide) {
                    player.direction += PI/2;
                    player.health -= 0.1;
                    can_collide = false;
                }

                if (checkCollision(projectile.shape, Asteroids[i]->shape)) {
                    Asteroids[i]->shape->render = GL_TRIANGLE_FAN;
                    shot = false;
                }
                
                glUniform4fv(GameColor, 1, value_ptr(vec4(clear_color[0], clear_color[1], clear_color[2], 0.0)));
                glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Asteroids[i]->shape->Model));
                glUniform1i(loc_flagP, 0);
                glLineWidth(2.0);

                glBindVertexArray(Asteroids[i]->shape->VAO);
                glDrawArrays(Asteroids[i]->shape->render, 0, Asteroids[i]->shape->vertices.size());
                glBindVertexArray(0);
                

                Asteroids[i]->shape->render = GL_LINE_LOOP;
                pacmanEffect(Asteroids[i], 20.0);
            }
            

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Renderizza i dati di disegno di ImGui
            glfwSwapBuffers(window);

            // only set lastFrameTime when you actually draw something
            lastFrameTime = now;
        }

        // set lastUpdateTime every iteration
        lastUpdateTime = now;
    }

    close_GUI();

    glDeleteProgram(fgShaders);
    

    glDeleteBuffers(1, &player.shape->VBO_vertices);
    glDeleteBuffers(1, &player.shape->VBO_colors);
    glDeleteVertexArrays(1, &player.shape->VAO);


    glfwTerminate();


    return 0;
}
