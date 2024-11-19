#pragma once
#include "strutture.h"
#include "gestione_curve.h"
#include "Utilities.h"

#define PI 3.14159265358979323


void init_player_shape(Shape* player) {

	//creo il player immettendo i vertici di controllo a mano e costruendo con essi una curva di Hermite

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

	//un semplice quadrato con 4 vertici. non uso i vertici di controllo né l'algoritmo di Hemrmite

	background->vertices.clear();

	background->vertices.push_back(vec3(-1.0,1.0, 0.0));
	background->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
	background->vertices.push_back(vec3(1.0, 1.0, 0.0));
	background->colors.push_back(vec4(0.0, 0.0, 0.0, 1.0));
	background->vertices.push_back(vec3(1.0, -1.0, 0.0));
	background->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	background->vertices.push_back(vec3(-1.0, -1.0, 0.0));
	background->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

	background->nv = background->vertices.size();
	background->render = GL_TRIANGLE_FAN;
}


void init_asteroid_shape(Shape* asteroid, float radius) {
	float step = PI / 4;
	float r = 0;
	vec3 vertex;
	vec3 first_vertex;
	bool first = true;

	asteroid->vertices.clear();
	asteroid->Derivata.clear();
	asteroid->CP.clear();
	asteroid->colors.clear();

	for (int t = 0; t < 8; t++) //uso un ciclo per disporre i vertici di controllo in cerchio
	{
		r = radius + ((rand() % 100) / 100.0)*radius; //i vertici vengono disposti radialmente a raggi diversi per avere una forma più realistica
		vertex = vec3((float)cos(t*step)*r,(float)sin(t*step)*r,0.0);
		if (first) {
			first_vertex = vertex;
			first = false;
		}
		asteroid->CP.push_back(vertex);
		asteroid->Derivata.push_back(vec3(0.0,0.0,0.0));
	}
	asteroid->CP.push_back(first_vertex); //per completare la forma dell'asteroide aggiungo di nuovo il primo vertice immesso
	asteroid->Derivata.push_back(vec3(0.0, 0.0, 0.0));

	asteroid->nv = 80;

	//con i vertici di controllo disposti dal ciclo creo la curva di Hermite

	CostruisciHermite(asteroid, vec4(1.0, 1.0, 1.0, 1.0));

	asteroid->render = GL_LINE_LOOP;
}

void init_projectile_shape(Shape* projectile) {


	//un semplice quadrato con 4 vertici. non uso i vertici di controllo né l'algoritmo di Hemrmite

	projectile->vertices.clear();

	projectile->vertices.push_back(vec3(-0.1, 0.3, 0.0));
	projectile->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));
	projectile->vertices.push_back(vec3(0.1, 0.3, 0.0));
	projectile->colors.push_back(vec4(0.0, 0.0, 0.0, 1.0));
	projectile->vertices.push_back(vec3(0.1, -0.3, 0.0));
	projectile->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
	projectile->vertices.push_back(vec3(-0.1, -0.3, 0.0));
	projectile->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

	projectile->nv = projectile->vertices.size();
	projectile->render = GL_TRIANGLE_FAN;
}