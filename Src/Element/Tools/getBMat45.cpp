//Author		: Xiao Tuzi
//Date(Create)	: 08/20/2022
//Description	: this program is definition of getting B matrix for Elem45

#include "Element/Tools/getBMat45.h"

void getBMat45(int nElemNode, dMatrix& Dshp, dMatrix& BMat) {
	// calculate B Matrix(strain-disp matrix)

	int u, v, w = 0;
	for (int i = 0; i < nElemNode; i++) {
		u = 1 + w;
		v = 1 + u;
		w = 1 + v;

		BMat(0, u - 1) = Dshp(0, i);
		BMat(3, u - 1) = Dshp(1, i);
		BMat(5, u - 1) = Dshp(2, i);

		BMat(1, v - 1) = Dshp(1, i);
		BMat(3, v - 1) = Dshp(0, i);
		BMat(4, v - 1) = Dshp(2, i);

		BMat(2, w - 1) = Dshp(2, i);
		BMat(4, w - 1) = Dshp(1, i);
		BMat(5, w - 1) = Dshp(0, i);
	}
}