#pragma once


void INIT_SHADER(void);
void INIT_VAO_DYNAMIC_Curva(Shape* fig);
void UPDATE_VAO_Curva(Shape* fig);

void init_player_actor(Actor* player);
void init_projectile_actor(Actor* projectile);
void init_background_actor(Actor* background);
Actor* init_asteroid(int initialR, int index);

