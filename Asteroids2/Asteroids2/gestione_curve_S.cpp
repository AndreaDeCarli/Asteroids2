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
	//Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente x della curva è quello originale, altrimenti è quello che è stato modificato nella funzione 
	//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

	if (curva->Derivata.at(i).x == 0)
		return dx(i, t,0.0,0.0,0.0, curva);
	
	if (curva->Derivata.at(i).x != 0)
		return curva->Derivata.at(i).x;

}

float DY(int i, float* t, Shape* curva)
{
	// Nei vertici di controllo per i quali non sono stati modificati i parametri Tens, Bias, Cont il valore della derivata della componente y della curva è quello originale, altrimenti è quello che è stato modificato nella funzione
		//keyboardfunc  in seguito alla modifica dei valori Tens, Bias e Cont.

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
		//Localizzare tg nell'intervallo [t[is],t[is+1]]
		if (tg > t[is + 1])is++;
		ampiezza = t[is + 1] - t[is];

		//Mappo tg appartenente a [t[is],t[is+1]] in [0,1] --> ottengo tm
		tm = (tg - t[is]) / ampiezza;

		//ampiezza dell'intervallo [t[is],t[is+1]] = t[is+1] - t[is]

		//valuto PH_x(t) = PHI_[is](tm) = x_[is]*PHI0(tm)+x[is+1]*PSI0(tm)+dx[is]*PHI1(tm)*ampiezza + dx[is+1]*PHI1(tm)*ampiezza
		x = curva->CP[is].x * PHI0(tm) + curva->CP[is + 1].x * PSI0(tm) + DX(is, t, curva) * PHI1(tm) * ampiezza + DX(is + 1, t, curva) * PSI1(tm) * ampiezza;

		//valuto PH_y(t) = PHI_[is](tm) = y_[is]*PHI0(tm)+y[is+1]*PSI0(tm)+dy[is]*PHI1(tm)*ampiezza + dy[is+1]*PHI1(tm)*ampiezza
		y = curva->CP[is].y * PHI0(tm) + curva->CP[is + 1].y * PSI0(tm) + DY(is, t, curva) * PHI1(tm) * ampiezza + DY(is + 1, t, curva) * PSI1(tm) * ampiezza;


		curva->vertices.push_back(vec3(x,y,0.0)); 
		curva->colors.push_back(color);
		

	}
	
}

void CostruisciHermite( Shape* curva, vec4 color)
{
	curva->vertices.clear();
	curva->colors.clear();
	InterpolazioneHermite(curva, color);
}