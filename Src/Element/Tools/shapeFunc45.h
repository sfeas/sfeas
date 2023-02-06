//Author		: Xiao Tuzi
//Date(Create)	: 08/20/2022
//Description	: this program is declaration of calculating shape function for Elem45


#pragma once

#include "DataType/Matrix.h"

void shapeFunc45(int iGP, int nElemNode, double* XP, double* fshape, dMatrix& Dshape);