#pragma once
#include <glad/glad.h>
#include "ShaderMaker.h"
#include "strutture.h"
#include "geometria.h"
#include "Utilities.h"

#define PI 3.14159265358979323

extern unsigned int fgShaders, bgShaders;
extern float asteroidR[3];

void INIT_SHADER(void)
{
    GLenum ErrorCheckValue = glGetError();

    char* vertexShader = (char*)"vertexshaderM.glsl";
    char* fragmentShader = (char*)"fragmentshaderM.glsl";
    char* fragmentShaderBG = (char*)"fragmentshaderSpace.glsl";
    
    fgShaders = ShaderMaker::createProgram(vertexShader, fragmentShader);




    bgShaders = ShaderMaker::createProgram(vertexShader, fragmentShaderBG);
}

void INIT_VAO_DYNAMIC_Curva(Shape* fig)
{

    glGenVertexArrays(1, &fig->VAO);
    glBindVertexArray(fig->VAO);
     
    //Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec3), fig->vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec4), fig->colors.data(), GL_DYNAMIC_DRAW);
    //Adesso carico il VBO dei colori nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
     


    
}
void UPDATE_VAO_Curva(Shape* fig)
{

   

    glBindVertexArray(fig->VAO);
    //Genero , rendo attivo, riempio il VBO della geometria dei vertici di COntrollo
     
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->vertices.size() * sizeof(vec3), fig->vertices.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    //Genero , rendo attivo, riempio il VBO dei colori nei vertici di controllo
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->colors.size() * sizeof(vec4), fig->colors.data());
    //Adesso carico il VBO dei colori nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
     
}


void init_player_actor(Actor* player) {
    player->isAlive = true;
    player->health = 1.0;
    player->velocity = 0.0;
    player->direction = 0.0;
    player->position.x = 0.0;
    player->position.y = 0.0;
    player->shape = new Shape;
    init_player_shape(player->shape);

    findBB(player->shape);
}

void init_background_actor(Actor* background) {
    background->velocity = 0.0;
    background->direction = 0.0;
    background->position.x = 0.0;
    background->position.y = 0.0;

    background->shape = new Shape;
    init_background_shape(background->shape);
}

Actor* init_asteroid(int initialR, int index) {
    Actor* asteroid = new Actor;
    asteroid->index = index;
    asteroid->health = 1.0;
    asteroid->isAlive = true;
    asteroid->radius_index = initialR;

    asteroid->direction = (rand()%100)/100.0 * 2 * PI;
    asteroid->position.x = ((rand() % 100) / 100.0 * 40) - 20;
    asteroid->position.y = ((rand() % 100) / 100.0 * 40) - 20;

    asteroid->velocity = 0.01;
    asteroid->shape = new Shape;
    init_asteroid_shape(asteroid->shape, asteroidR[asteroid->radius_index]);

    findBB(asteroid->shape);

    return asteroid;
}

void init_projectile_actor(Actor* projectile) {

    projectile->health = 1.0;

    projectile->velocity = 1.5;
    projectile->direction = 0.0;

    projectile->position.x = 0.0;
    projectile->position.y = 0.0;

    projectile->shape = new Shape;
    init_projectile_shape(projectile->shape);

    findBB(projectile->shape);

}

