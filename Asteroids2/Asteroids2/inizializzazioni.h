#pragma once

/*
#pragma once //: � una direttiva che indica al compilatore di includere il file una sola volta per unit� di compilazione.
//  Il meccanismo esatto varia a seconda del compilatore, ma in generale si basa su un meccanismo interno per tenere traccia dei file gi� inclusi.
//Non � parte dello standard C++, quindi la sua disponibilit� e il comportamento preciso possono variare tra diversi compilatori. Tuttavia, � supportata dalla maggior parte dei compilatori moderni.
*/


 void INIT_SHADER(void);
void INIT_VAO_DYNAMIC_Curva(Shape* fig);
void UPDATE_VAO_Curva(Shape* fig);

void init_player_actor(Actor* player);
void init_background_actor(Actor* background);
Actor* init_asteroid(float initialR);

