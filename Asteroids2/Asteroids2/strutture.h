/*
#pragma once //: È una direttiva che indica al compilatore di includere il file una sola volta per unità di compilazione.
//  Il meccanismo esatto varia a seconda del compilatore, ma in generale si basa su un meccanismo interno per tenere traccia dei file già inclusi.
//Non è parte dello standard C++, quindi la sua disponibilità e il comportamento preciso possono variare tra diversi compilatori. Tuttavia, è supportata dalla maggior parte dei compilatori moderni.
*/


#pragma once

#include "lib.h"

typedef struct {
    // Identificatori OpenGL per la gestione della geometria e degli attributi
    GLuint VAO;                // Vertex Array Object
    GLuint VBO_vertices;       // Vertex Buffer Object per le coordinate dei vertici della curva
    GLuint VBO_colors;

    // Dati geometrici della curva
    std::vector<glm::vec3> vertices;  // Coordinate dei vertici della curva
    std::vector<glm::vec4> colors;    // Colori dei vertici della curva
    std::vector<glm::vec3> CP;        // Coordinate dei control points (punti di controllo)
    std::vector<glm::vec4> colCP;     // Colori dei control points

    // Parametri di interpolazione
    std::vector<glm::vec3> Derivata;  // Vettore derivata (a cosa serve esattamente?)


    // Informazioni sulla curva
    int nv;                     // Numero di vertici della curva
    glm::mat4 Model;            // Matrice di modellazione

    // Parametri di rendering
    int render;                // Modalità di rendering (linee, triangoli, ...)
    unsigned int programId;    // Identificatore del programma shader

} Shape;

typedef struct {
    float x;
    float y;
}Position;

typedef struct {
    float velocity;
    float direction;
    Position position;
    Shape* shape;
}Actor;

