#pragma once
#include "strutture.h"
float dx(int i, float* t, float Tens, float Bias, float Cont, Curva* curva);
float dy(int i, float* t, float Tens, float Bias, float Cont, Curva* curva);
float DX(int i, float* t);
float DY(int i, float* t);
void InterpolazioneHermite(Curva* curva, vec4 color);
void CostruisciHermite( Curva* curva, vec4 color);