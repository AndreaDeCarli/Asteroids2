#pragma once
#include "strutture.h"
float dx(int i, float* t, float Tens, float Bias, float Cont, Shape* curva);
float dy(int i, float* t, float Tens, float Bias, float Cont, Shape* curva);
float DX(int i, float* t, Shape* curva);
float DY(int i, float* t, Shape* curva);
void InterpolazioneHermite(Shape* curva, vec4 color);
void CostruisciHermite( Shape* curva, vec4 color);