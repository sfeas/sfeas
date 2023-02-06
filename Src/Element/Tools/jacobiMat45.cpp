//Author		: Xiao Tuzi
//Date(Create)	: 08/20/2022
//Description	: this program is definition of calculating jacobi matrix for Elem45


#include "Element/Tools/jacobiMat45.h"
#include "Element/Tools/getDetInvMatrix.h"
#include "MathSolver/MathSolver.h"

void jacobiMat45(int nElemNode, dMatrix& ElemXYZ, dMatrix& Dshape, double& detJ, dMatrix& invJ) {
	// calculate determinantand and inverse of element jocobi matrix

	// Jacobi = Dshape * ElemXYZ
	dMatrix Jacobi(3, 3);
	MS.dgemm(Dshape.datas(), ElemXYZ.datas(), Jacobi.datas(), 3, nElemNode, 3);

	// calculate det and inv of Jacobi
	getDetInvMatrix(Jacobi, detJ, invJ);
}