//Author		: Xiao Tuzi
//Date(Create)	: 08/20/2022
//Description	: this program is declaration of calculating jacobi matrix for Elem45


#pragma once

#include "DataType/Matrix.h"

void jacobiMat45(int nElemNode, dMatrix& ElemXYZ, dMatrix& Dshape, double& detJ, dMatrix& invJ);