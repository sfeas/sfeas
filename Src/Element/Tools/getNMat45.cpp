//Author		: Xiao Tuzi
//Date(Create)	: 08/21/2022
//Description	: this program is definition of getting N Matrix for Elem45


#include "Element/Tools/getNMat45.h"

void getNMat45(const int nElemNode, double* fshape, dMatrix& NMat) {
	// calculate N Matrix(shape function matrix)

	for (int i = 0; i < nElemNode; i++) {
		NMat(0, 3 * i + 0) = fshape[i];
		NMat(1, 3 * i + 1) = fshape[i];
		NMat(2, 3 * i + 2) = fshape[i];
	}
}