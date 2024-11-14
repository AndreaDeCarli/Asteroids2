#pragma once
#include "strutture.h"
#include "lib.h"

void findBB(Shape* fig);
bool checkCollision(Shape* obj1, Shape* obj2);
void updateBB(Shape* fig);
void pacmanEffect(Actor* actor, float limits);
bool outsideBoundary(Actor* actor, float limits);