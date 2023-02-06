//Author		: Xiao Tuzi
//Date(Create)	: 08/20/2022
//Description	: this program is definition of calculating shape function for Elem45

#include "Element/Tools/shapeFunc45.h"
#include "DataType/Matrix.h"

void shapeFunc45(int iGP, int nElemNode, double* XP, double* fshape, dMatrix& Dshape) {
	// Calculate shape functions(fshape) and their derivatives for local coordinate(Dshape)
	// fshape = [N1, N2, N3, N4, N5, N6, N7, N8]

	// Dshape = | dN1 / dr, ..., dN8 / dr |
	//          | dN1 / ds, ..., dN8 / ds |
	//          | dN1 / dt, ..., dN8 / dt |

	/*			(3D8 Hexa)
			   P！！！！！！！！！O
			  /|       / |
			 / |      /  |
			/  |L____/___|K
		   M！！！/！！！！！N   /
		   |  /      |  /          t
		   | /       | /           |  /s
		   |/        |/            | /
		   I！！！！！！！！！J             |/______ r

	*/

	double r = XP[0];
	double s = XP[1];
	double t = XP[2];

	switch (nElemNode) {
	case 4:			    // (3D4 Tetra), K=L, M=N=O=P
		// shape function for local coordinate(3D4 nodes)
		fshape[0] = 1.0 - r - s - t;
		fshape[1] = r;
		fshape[2] = s;
		fshape[3] = t;

		// shape function derivative for local coordinate(3D4 nodes)
		Dshape(0, 0) = -1.0;
		Dshape(1, 0) = -1.0;
		Dshape(2, 0) = -1.0;

		Dshape(0, 1) = 1.0;
		Dshape(1, 1) = 0.0;
		Dshape(2, 1) = 0.0;

		Dshape(0, 2) = 0.0;
		Dshape(1, 2) = 1.0;
		Dshape(2, 2) = 0.0;

		Dshape(0, 3) = 0.0;
		Dshape(1, 3) = 0.0;
		Dshape(2, 3) = 1.0;
		break;
	case 6:			    // (3D6 Wedge), K=L, O=P
		// shape function for local coordinate(3D6 nodes)
		fshape[0] = 0.5 * (1 - r - s) * (1 - t);
		fshape[1] = 0.5 * r * (1 - t);
		fshape[2] = 0.5 * s * (1 - t);
		fshape[3] = 0.5 * (1 - r - s) * (1 + t);
		fshape[4] = 0.5 * r * (1 + t);
		fshape[5] = 0.5 * s * (1 + t);

		// shape function derivative for local coordinate(3D6 nodes)
		Dshape(0, 0) = -0.5 * (1 - t);
		Dshape(1, 0) = -0.5 * (1 - t);
		Dshape(2, 0) = -0.5 * (1 - r - s);

		Dshape(0, 1) = 0.5 * (1 - t);
		Dshape(1, 1) = 0.0;
		Dshape(2, 1) = -0.5 * r;

		Dshape(0, 2) = 0.0;
		Dshape(1, 2) = 0.5 * (1 - t);
		Dshape(2, 2) = -0.5 * s;

		Dshape(0, 3) = -0.5 * (1 + t);
		Dshape(1, 3) = -0.5 * (1 + t);
		Dshape(2, 3) = 0.5 * (1 - r - s);

		Dshape(0, 4) = 0.5 * (1 + t);
		Dshape(1, 4) = 0.0;
		Dshape(2, 4) = 0.5 * r;

		Dshape(0, 5) = 0.0;
		Dshape(1, 5) = 0.5 * (1 + t);
		Dshape(2, 5) = 0.5 * s;
		break;
	case 8:			    // (3D8 Hexa)
		// shape function for local coordinate(3D8 nodes)
		fshape[0] = 0.125 * (1 - r) * (1 - s) * (1 - t);
		fshape[1] = 0.125 * (1 + r) * (1 - s) * (1 - t);
		fshape[2] = 0.125 * (1 + r) * (1 + s) * (1 - t);
		fshape[3] = 0.125 * (1 - r) * (1 + s) * (1 - t);
		fshape[4] = 0.125 * (1 - r) * (1 - s) * (1 + t);
		fshape[5] = 0.125 * (1 + r) * (1 - s) * (1 + t);
		fshape[6] = 0.125 * (1 + r) * (1 + s) * (1 + t);
		fshape[7] = 0.125 * (1 - r) * (1 + s) * (1 + t);

		// shape function derivative for local coordinate(3D8 nodes)
		Dshape(0, 0) = -0.125 * (1 - s) * (1 - t);
		Dshape(1, 0) = -0.125 * (1 - r) * (1 - t);
		Dshape(2, 0) = -0.125 * (1 - r) * (1 - s);

		Dshape(0, 1) = 0.125 * (1 - s) * (1 - t);
		Dshape(1, 1) = -0.125 * (1 + r) * (1 - t);
		Dshape(2, 1) = -0.125 * (1 + r) * (1 - s);

		Dshape(0, 2) = 0.125 * (1 + s) * (1 - t);
		Dshape(1, 2) = 0.125 * (1 + r) * (1 - t);
		Dshape(2, 2) = -0.125 * (1 + r) * (1 + s);

		Dshape(0, 3) = -0.125 * (1 + s) * (1 - t);
		Dshape(1, 3) = 0.125 * (1 - r) * (1 - t);
		Dshape(2, 3) = -0.125 * (1 - r) * (1 + s);

		Dshape(0, 4) = -0.125 * (1 - s) * (1 + t);
		Dshape(1, 4) = -0.125 * (1 - r) * (1 + t);
		Dshape(2, 4) = 0.125 * (1 - r) * (1 - s);

		Dshape(0, 5) = 0.125 * (1 - s) * (1 + t);
		Dshape(1, 5) = -0.125 * (1 + r) * (1 + t);
		Dshape(2, 5) = 0.125 * (1 + r) * (1 - s);

		Dshape(0, 6) = 0.125 * (1 + s) * (1 + t);
		Dshape(1, 6) = 0.125 * (1 + r) * (1 + t);
		Dshape(2, 6) = 0.125 * (1 + r) * (1 + s);

		Dshape(0, 7) = -0.125 * (1 + s) * (1 + t);
		Dshape(1, 7) = 0.125 * (1 - r) * (1 + t);
		Dshape(2, 7) = 0.125 * (1 - r) * (1 + s);
		break;
	default:
		break;
	}

	return;
}
