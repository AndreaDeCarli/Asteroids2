#pragma once
#include "strutture.h"
#include "lib.h"

void findBB(Shape* fig);
bool checkCollision(Shape* obj1, Shape* obj2);
void updateBB(Shape* fig);