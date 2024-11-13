#pragma once
#include "strutture.h"
#include "gestione_curve.h"

#define PI 3.14159265358979323


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


void init_background_shape(Shape* background) {

	background->vertices.clear();

	background->vertices.push_back(vec3(-1.0,1.0, 0.0));
	background->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
	background->vertices.push_back(vec3(1.0, 1.0, 0.0));
	background->colors.push_back(vec4(0.0, 0.0, 0.0, 1.0));
	background->vertices.push_back(vec3(1.0, -1.0, 0.0));
	background->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	background->vertices.push_back(vec3(-1.0, -1.0, 0.0));
	background->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

	background->nv = background->vertices.size();
	background->render = GL_TRIANGLE_FAN;
}


void init_asteroid_shape(Shape* asteroid, float initialR) {
	float step = PI / 4;
	float r = 0;
	vec3 vertex;

	for (int t = 0; t < 8; t++)
	{
		r = initialR + (rand() % 100) / 100.0;
		vertex = vec3((float)cos(t*step)*r,(float)sin(t*step)*r,0.0);
		asteroid->CP.push_back(vertex);
		asteroid->Derivata.push_back(vec3(0.0,0.0,0.0));
	}
	asteroid->nv = 80;

	CostruisciHermite(asteroid, vec4(1.0, 1.0, 1.0, 1.0));

	asteroid->render = GL_LINE_LOOP;
}
