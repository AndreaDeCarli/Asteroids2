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
#define COLLISION_TIMEOUT 20
#define SCENE_LIMITS 20.0

//Variabili globali -------------------------------------------------------------------
unsigned int fgShaders, bgShaders;
float r = 0.0, g = 0.0, b = 0.0, alpha, scale_factor;
int height = 1000, width = 1000, collision_timer = 0, numberOfAsteroids = 10, score = 0;
Actor player = {}, background = {}, projectile = {};
Actor* Asteroids[40] = {};
mat4 Projection;
GLuint MatProj, MatModel, loc_flagP, GameColor, vec_resS, loc_time;
float clear_color[3] = { 1.0, 1.0, 1.0 }, asteroidR[3] = {1.5, 0.75, 0.3};
bool acc = false, TURN_LEFT = false, TURN_RIGHT = false, can_collide = true, shot = false;
const double fpsLimit = 1.0 / 60.0; //limitatore dei frame per secondo
double lastUpdateTime = 0;  // numero di secondi passati dall'ultimo loop
double lastFrameTime = 0;   // numero di secondi passati dall'ultimo frame

//----------------------------------------------------------------------------------------

int main(void)
{
    GLFWwindow* window;

    
    if (!glfwInit())   //if glfw is not loaded return -1
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Enable double buffering

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    
    window = glfwCreateWindow(height, width, "Asteroids", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create the window !" << std::endl;
        glfwTerminate(); 
        return -1;
    }

    
    glfwMakeContextCurrent(window); //crea il context corrente e lo associa a window. In opengl un rendering context è una macchina astati che memorizza tutte le informazioni necessarie e le risorse per il rendering grafico


    //verifica se la libreria GLAD è riuscita a caricare correttamente tutti i puntatori 
    // alle funzioni OpenGL necessarie.

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load opengl function pointers !" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, cursor_position_callback);



    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    INIT_SHADER();

    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwSetMouseButtonCallback(window, mouse_button_callback);


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    
    scale_factor = 0.05f;

    init_player_actor(&player);          //inizializzo la struttura del player
    init_background_actor(&background);  //inizializzo la struttura del background
    init_projectile_actor(&projectile);  //inizializzo la struttura del proiettile

    
    for (int i = 0; i < 10; i++)         //inizializzo le strutture degli asteroidi
    {
        Asteroids[i] = init_asteroid(0);
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

        if ((now - lastFrameTime) >= fpsLimit) //limita il framerate a quelo desiderato
        {

            if (acc)
                player.velocity += 0.005f; //la velocità viene aumentata per tutto il tempo in cui la variabile di accelerazione è true
            else if (player.velocity > 0) 
                player.velocity -= 0.003f; //altrimenti la velocità diminuisce costantemente
            if (player.velocity < 0) 
                player.velocity = 0;

            if (TURN_LEFT) 
                player.direction += 2 * (float)PI * 0.01f;
            if (TURN_RIGHT) 
                player.direction -= 2 * (float)PI * 0.01f;

            if (can_collide == false) {
                collision_timer += 1;
                if (collision_timer >= COLLISION_TIMEOUT) {
                    can_collide = true;
                    collision_timer = 0;
                    player.shape->render = GL_LINE_LOOP;
                }

            }

            player.position.x += (player.velocity * cos(player.direction));
            player.position.y += (player.velocity * sin(player.direction));


            glClearColor(0.0, 0.0, 0.0, 0.0);
            glClear(GL_COLOR_BUFFER_BIT);
            my_interface();

            //BACKGROUND------------------------------------------------------------------------------------

            glUseProgram(bgShaders); //viene usato un altro shader per il background
            
            background.shape->Model = mat4(1.0);
            background.shape->Model = scale(background.shape->Model, vec3(float(width) * 2.0, float(height) * 2.0, 1.0));

            glUniform4fv(GameColor,1, value_ptr(vec4(clear_color[0], clear_color[1], clear_color[2], 0.0)));
            glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
            glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(background.shape->Model));
            glUniform2fv(vec_resS, 1, value_ptr(vec2((float)height, (float)width)));
            glUniform1f(loc_time, (float)now);

            glBindVertexArray(background.shape->VAO);
            glDrawArrays(background.shape->render, 0, background.shape->nv);

            //PLAYER------------------------------------------------------------------------------------

            if (player.isAlive) {
                glUseProgram(fgShaders);

                glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
                player.shape->Model = mat4(1.0);
                player.shape->Model = scale(player.shape->Model, vec3(scale_factor, scale_factor, 1.0));
                player.shape->Model = translate(player.shape->Model, vec3(player.position.x, player.position.y, 0.0));
                updateBB(player.shape);
                player.shape->Model = rotate(player.shape->Model, player.direction - (float)PI / 2, vec3(0.0, 0.0, 1.0));
                glUniform4fv(GameColor, 1, value_ptr(vec4(clear_color[0], clear_color[1], clear_color[2], 0.0)));
                glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(player.shape->Model));
                glUniform1i(loc_flagP, 0);
                glLineWidth(2.0);

                glBindVertexArray(player.shape->VAO);
                glDrawArrays(player.shape->render, 0, player.shape->vertices.size());
                glBindVertexArray(0);

                
                pacmanEffect(&player, 20.0);
            }

            //PROIETTILE------------------------------------------------------------------------------------

            if (shot) { //solo se il proiettile è stato sparato aggiornerà la sua posizione nella direzione in cui è stato sparato
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

                if (outsideBoundary(&projectile, SCENE_LIMITS)) { //se il proiettile va fuori dai limiti smette di essere "sparato"
                    shot = false;
                }

            }
            else {
                //se il proiettile non è sparato allora aggiorna costantemente la sua posizione e direzione a quelle del player
                projectile.position.x = player.position.x;
                projectile.position.y = player.position.y;
                projectile.direction = player.direction;
                projectile.shape->Model = mat4(1.0);
                projectile.shape->Model = scale(projectile.shape->Model, vec3(scale_factor, scale_factor, 1.0));
                projectile.shape->Model = translate(projectile.shape->Model, vec3(projectile.position.x, projectile.position.y, 0.0));
                updateBB(projectile.shape);
            }

            //ASTEROIDI------------------------------------------------------------------------------------

            for (int i = 0; i < numberOfAsteroids; i++){ // ciclo su tutti gli asteroidi

                
                if (Asteroids[i]->isAlive) { //solo se gli asteroidi sono vivi vengono renderizzati

                    if (Asteroids[i]->health <= 0)
                    {
                        score += (Asteroids[i]->radius_index + 1) * 100;
                        if (Asteroids[i]->radius_index == 2) { //se l'asteroide è più piccolo di un certo threshold viene eliminato
                            Asteroids[i]->isAlive = false;
                        }
                        else { //se l'asteroide viene distrutto ne genera altri due
                            float original_direction = Asteroids[i]->direction;
                            float asteroid_x = Asteroids[i]->position.x;
                            float asteroid_y = Asteroids[i]->position.y;
                            Asteroids[i]->radius_index++;
                            init_asteroid_shape(Asteroids[i]->shape, asteroidR[Asteroids[i]->radius_index]);
                            INIT_VAO_DYNAMIC_Curva(Asteroids[i]->shape);
                            findBB(Asteroids[i]->shape);
                            Asteroids[numberOfAsteroids] = init_asteroid(Asteroids[i]->radius_index);
                            INIT_VAO_DYNAMIC_Curva(Asteroids[numberOfAsteroids]->shape);
                            findBB(Asteroids[numberOfAsteroids]->shape);
                            Asteroids[numberOfAsteroids]->position.x = asteroid_x;
                            Asteroids[numberOfAsteroids]->position.y = asteroid_y;
                            Asteroids[numberOfAsteroids]->direction = original_direction + (float)PI / 6;
                            Asteroids[i]->direction = original_direction - (float)PI / 6;
                            Asteroids[i]->health = 1.0;
                            //aumenta la velocità dell'asteroide più piccolo
                            Asteroids[numberOfAsteroids]->velocity += 0.05f;
                            Asteroids[i]->velocity = Asteroids[numberOfAsteroids]->velocity;

                            numberOfAsteroids++;
                        }
                    }
                    //aggiornamento delle coordinate del singolo asteroide
                    Asteroids[i]->position.x += (Asteroids[i]->velocity * cos(Asteroids[i]->direction));
                    Asteroids[i]->position.y += (Asteroids[i]->velocity * sin(Asteroids[i]->direction));

                    glUseProgram(fgShaders);

                    glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
                    Asteroids[i]->shape->Model = mat4(1.0);
                    Asteroids[i]->shape->Model = scale(Asteroids[i]->shape->Model, vec3(scale_factor, scale_factor, 1.0));
                    Asteroids[i]->shape->Model = translate(Asteroids[i]->shape->Model, vec3(Asteroids[i]->position.x, Asteroids[i]->position.y, 0.0));
                    updateBB(Asteroids[i]->shape);

                    Asteroids[i]->shape->Model = rotate(Asteroids[i]->shape->Model, (float)now*(float)0.1, vec3(0.0, 0.0, 1.0));

                    //controllo se un asteroide colpisce il player
                    if (checkCollision(player.shape, Asteroids[i]->shape) && can_collide) {
                        player.direction += (float)PI / 2; //la direzione del player viene cambiata di 90 gradi
                        player.health -= 0.1f; //la salute del player viene diminuita
                        if (player.health <= 0) {
                            player.isAlive = false;
                        }

                        //se il player viene colpito da un asteroide mentre è fermo acquisisce velocità
                        if (player.velocity <= 0.1) {
                            player.velocity = 0.2f;
                        }
                        //se il player viene colpito da un asteroide mentre è in movimento perde velocità
                        else {
                            player.velocity = player.velocity * 0.75f;
                        }
                        //il player viene renderizzato con colore pieno per visualizzare il danno subito
                        player.shape->render = GL_TRIANGLE_FAN;
                        can_collide = false;
                    }
                    //controllo se un asteroide è stato colpito dal proiettile
                    if (checkCollision(projectile.shape, Asteroids[i]->shape) && shot) {
                        Asteroids[i]->health -= 1.0;
                        Asteroids[i]->shape->render = GL_TRIANGLE_FAN; //se un asteroide viene colpito viene colorato anche dentro
                        shot = false; //il proiettile deve smettere di muoversi e tornare al player
                    }

                    glUniform4fv(GameColor, 1, value_ptr(vec4(clear_color[0], clear_color[1], clear_color[2], 0.0)));
                    glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(Asteroids[i]->shape->Model));
                    glUniform1i(loc_flagP, 0);
                    glLineWidth(2.0);

                    glBindVertexArray(Asteroids[i]->shape->VAO);
                    glDrawArrays(Asteroids[i]->shape->render, 0, Asteroids[i]->shape->vertices.size());
                    glBindVertexArray(0);


                    Asteroids[i]->shape->render = GL_LINE_LOOP;
                    pacmanEffect(Asteroids[i], SCENE_LIMITS + 2.0);
                }
                
            }
            
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Renderizza la GUI di ImGui
            glfwSwapBuffers(window);

            
            lastFrameTime = now;
        }

        lastUpdateTime = now;
    }

    close_GUI();

    glDeleteProgram(fgShaders);
    glDeleteProgram(bgShaders);
    

    glDeleteBuffers(1, &player.shape->VBO_vertices);
    glDeleteBuffers(1, &player.shape->VBO_colors);
    glDeleteVertexArrays(1, &player.shape->VAO);


    glDeleteBuffers(1, &background.shape->VBO_vertices);
    glDeleteBuffers(1, &background.shape->VBO_colors);
    glDeleteVertexArrays(1, &background.shape->VAO);


    glDeleteBuffers(1, &projectile.shape->VBO_vertices);
    glDeleteBuffers(1, &projectile.shape->VBO_colors);
    glDeleteVertexArrays(1, &projectile.shape->VAO);

    for (int i = 0; i < numberOfAsteroids; i++)
    {
        glDeleteBuffers(1, &Asteroids[i]->shape->VBO_vertices);
        glDeleteBuffers(1, &Asteroids[i]->shape->VBO_colors);
        glDeleteVertexArrays(1, &Asteroids[i]->shape->VAO);
    }


    glfwTerminate();


    return 0;
}
