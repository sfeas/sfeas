//Author		: Xiao Tuzi
//Date(Create)	: 08/27/2022
//Description	: this program is declaration of updating U0 to NStat[i].U


#pragma once
#include "DataType/Matrix.h"

void updateU(double* U0);
void updateU(dMatrix DVA);
void updateU(zMatrix CDVA, double fNow, const int label = 0);