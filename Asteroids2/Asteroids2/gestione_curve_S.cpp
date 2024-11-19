#pragma once
#include "strutture.h"

//Per Curve di hermite
#define PHI0(t)  (2.0*t*t*t-3.0*t*t+1)
#define PHI1(t)  (t*t*t-2.0*t*t+t)
#define PSI0(t)  (-2.0*t*t*t+3.0*t*t)
#define PSI1(t)  (t*t*t-t*t)


float dx(int i, float* t, float Tens, float Bias, float Cont, Shape* curva)
{
	if (i == 0)
		return  0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP[i + 1].x - curva->CP[i].x) / (t[i + 1] - t[i]);
	if (i == curva->CP.size() - 1)
		return  0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP[i].x - curva->CP[i - 1].x) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 + Cont) * (curva->CP.at(i).x - curva->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (curva->CP.at(i + 1).x - curva->CP.at(i).x) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 - Cont) * (curva->CP.at(i).x - curva->CP.at(i - 1).x) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (curva->CP.at(i + 1).x - curva->CP.at(i).x) / (t[i + 1] - t[i]);
}
float dy(int i, float* t, float Tens, float Bias, float Cont, Shape* curva)
{
	if (i == 0)
		return 0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
	if (i == curva->CP.size() - 1)
		return  0.5 * (1.0 - Tens) * (1.0 - Bias) * (1.0 - Cont) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]);

	if (i % 2 == 0)
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 + Cont) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 - Cont) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
	else
		return  0.5 * (1.0 - Tens) * (1.0 + Bias) * (1.0 - Cont) * (curva->CP.at(i).y - curva->CP.at(i - 1).y) / (t[i] - t[i - 1]) + 0.5 * (1 - Tens) * (1 - Bias) * (1 + Cont) * (curva->CP.at(i + 1).y - curva->CP.at(i).y) / (t[i + 1] - t[i]);
}

float DX(int i, float* t, Shape* curva)
{
	if (curva->Derivata.at(i).x == 0)
		return dx(i, t,0.0,0.0,0.0, curva);
	
	if (curva->Derivata.at(i).x != 0)
		return curva->Derivata.at(i).x;

}

float DY(int i, float* t, Shape* curva)
{
	if (curva->Derivata.at(i).y == 0)
		return dy(i, t, 0.0, 0.0, 0.0, curva);

	if (curva->Derivata.at(i).y != 0)
		return curva->Derivata.at(i).y;

}

void InterpolazioneHermite(Shape* curva, vec4 color)
{
	
	float tg,ampiezza,tm,x,y,step_t;
	float* t;
	t = new float[curva->CP.size()];
	step_t = 1.0 / (curva->CP.size() - 1);
	for (int i = 0; i < curva->CP.size(); i++)
		t[i] = (float)i * step_t;

	t[curva->CP.size()] = 1.0;

	float stepTg = 1.0 / (float)(curva->nv - 1);
	int is = 0;

	

	for ( tg = 0; tg <= 1; tg+=stepTg)
	{
		if (tg > t[is + 1])is++;
		ampiezza = t[is + 1] - t[is];

		tm = (tg - t[is]) / ampiezza;


		x = curva->CP[is].x * PHI0(tm) + curva->CP[is + 1].x * PSI0(tm) + DX(is, t, curva) * PHI1(tm) * ampiezza + DX(is + 1, t, curva) * PSI1(tm) * ampiezza;

		y = curva->CP[is].y * PHI0(tm) + curva->CP[is + 1].y * PSI0(tm) + DY(is, t, curva) * PHI1(tm) * ampiezza + DY(is + 1, t, curva) * PSI1(tm) * ampiezza;


		curva->vertices.push_back(vec3(x,y,0.0)); 
		curva->colors.push_back(color);
	}
	
}

void CostruisciHermite( Shape* curva, vec4 color) //funzione che costruisce la curva di hermite dai vertici di controllo e le derivate
{
	curva->vertices.clear();
	curva->colors.clear();
	InterpolazioneHermite(curva, color);
}