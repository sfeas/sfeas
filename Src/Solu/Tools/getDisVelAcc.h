//Author		: Xiao Tuzi
//Date(Create)	: 09/08/2022
//Description	: this program is calculating new DisVelAcc in time integral algrithm of TASolu


#pragma once
#include "DataType/Matrix.h"

void getDisVelAcc(double* U, const double* B, dMatrix& disVelAcc);
