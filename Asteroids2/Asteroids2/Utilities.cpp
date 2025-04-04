#pragma once
#include "lib.h"
#include "strutture.h"


void findBB(Shape* fig) {
    int n;
    n = fig->vertices.size();
    float minx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float miny = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    float maxx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float maxy = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    for (int i = 1; i < n; i++) {
        if (fig->vertices[i].x < minx) {
            minx = fig->vertices[i].x;
        }
        if (fig->vertices[i].x > maxx) {
            maxx = fig->vertices[i].x;
        }

        if (fig->vertices[i].y < miny) {
            miny = fig->vertices[i].y;
        }

        if (fig->vertices[i].y > maxy) {
            maxy = fig->vertices[i].y;
        }
    }
     
    fig->min_BB_obj = vec4(minx,miny,0.0,1.0);
    fig->max_BB_obj = vec4(maxx, maxy, 0.0,1.0);

}

bool checkCollision(Shape* obj1, Shape* obj2) {
    // guardo collisioni su asse x

    bool collisionX = obj1->min_BB.x <= obj2->max_BB.x &&
        obj1->max_BB.x >= obj2->min_BB.x;
    
    // guardo collisioni su asse y

    bool collisionY = obj1->min_BB.y <= obj2->max_BB.y &&
        obj1->max_BB.y >= obj2->min_BB.y;

    
    //Si ha collisione se c'� collisione sia nella direzione x che nella direzione y

    return collisionX && collisionY;
}

void updateBB(Shape* fig)
{
    //Aggiorno le coordinate del Bounding Box applicandogli la stessa matrice di modellazione della sua shape
    fig->min_BB = fig->min_BB_obj;
    fig->max_BB = fig->max_BB_obj;
    fig->min_BB = fig->Model * fig->min_BB;
    fig->max_BB = fig->Model * fig->max_BB;
}

void pacmanEffect(Actor* actor, float limits) {
    //aggiorna la posizione di un personaggio se va oltre un certo limite dato in input
    if (actor->position.x < -limits) {
        actor->position.x = limits;
    }
    else if (actor->position.x > limits) {
        actor->position.x = -limits;
    }
    if (actor->position.y < -limits) {
        actor->position.y = limits;
    }
    else if (actor->position.y > limits) {
        actor->position.y = -limits;
    }
}

bool outsideBoundary(Actor* actor, float limits) {
    //return true se il personaggio dato in input � fuori dai limiti della scena immessi
    if (actor->position.x > limits || actor->position.x < -limits || actor->position.y > limits || actor->position.y < -limits) {
        return true;
    }
    return false;
}
