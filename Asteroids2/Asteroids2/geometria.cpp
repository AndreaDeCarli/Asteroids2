#pragma once
#include "strutture.h"
#include "gestione_curve.h"

void init_player_shape(Shape* player) {

	vec4 whiteColor = vec4(1.0, 1.0, 1.0, 1.0);

	player->CP.push_back(vec3(0.0f, 1.0f, 0.0f));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));
	player->CP.push_back(vec3(0.4f, 0.2f, 0.0f));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));

	player->CP.push_back(vec3(0.6, -0.4, 0.0));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));

	player->CP.push_back(vec3(0.6f, -0.8f, 0.0f));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));
	player->CP.push_back(vec3(0.0f, -0.5f, 0.0f));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));
	player->CP.push_back(vec3(-0.6f, -0.8f, 0.0f));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));

	player->CP.push_back(vec3(-0.6, -0.4, 0.0));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));

	player->CP.push_back(vec3(-0.4f, 0.2f, 0.0f));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));
	player->CP.push_back(vec3(0.0f, 1.0f, 0.0f));
	player->Derivata.push_back(vec3(0.0, 0.0, 0.0));

	player->nv = 70;

	CostruisciHermite(player, vec4(1.0, 1.0, 1.0, 1.0));

	player->render = GL_LINE_STRIP;
}

