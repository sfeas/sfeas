//Author		: Xiao Tuzi
//Date(Create)	: 08/20/2022
//Description	: this program is definition of calculating det and inv of matrix


#include "Print/PrintMsg.h"
#include "Element/Tools/getDetInvMatrix.h"

void getDetInvMatrix(dMatrix& Mat, double& detJ, dMatrix& invMat) {
	// calculate inverse of Matrix

	double j11, j12, j13, j21, j22, j23, j31, j32, j33;
	int ndim = Mat.rows();
	switch (ndim) {
	case 2:
		detJ = Mat(0, 0) * Mat(1, 1) - Mat(0, 1) * Mat(1, 0);
		if (detJ == 0.0) { printMsg.error("The det of Matrix is zero"); }

		invMat(0, 0) = Mat(1, 1) / detJ;
		invMat(1, 1) = Mat(0, 0) / detJ;
		invMat(0, 1) = -Mat(0, 1) / detJ;
		invMat(1, 0) = -Mat(1, 0) / detJ;
		break;
	case 3:
		detJ = Mat(0, 0) * (Mat(1, 1) * Mat(2, 2) - Mat(2, 1) * Mat(1, 2));
		detJ = detJ - Mat(0, 1) * (Mat(1, 0) * Mat(2, 2) - Mat(2, 0) * Mat(1, 2));
		detJ = detJ + Mat(0, 2) * (Mat(1, 0) * Mat(2, 1) - Mat(2, 0) * Mat(1, 1));
		if (detJ == 0.0) { printMsg.error("The det of Matrix is zero"); }

		j11 = Mat(1, 1) * Mat(2, 2) - Mat(2, 1) * Mat(1, 2);
		j21 = -Mat(1, 0) * Mat(2, 2) + Mat(2, 0) * Mat(1, 2);
		j31 = Mat(1, 0) * Mat(2, 1) - Mat(2, 0) * Mat(1, 1);
		j12 = -Mat(0, 1) * Mat(2, 2) + Mat(2, 1) * Mat(0, 2);
		j22 = Mat(0, 0) * Mat(2, 2) - Mat(2, 0) * Mat(0, 2);
		j32 = -Mat(0, 0) * Mat(2, 1) + Mat(2, 0) * Mat(0, 1);
		j13 = Mat(0, 1) * Mat(1, 2) - Mat(1, 1) * Mat(0, 2);
		j23 = -Mat(0, 0) * Mat(1, 2) + Mat(1, 0) * Mat(0, 2);
		j33 = Mat(0, 0) * Mat(1, 1) - Mat(1, 0) * Mat(0, 1);
		invMat(0, 0) = j11 / detJ;
		invMat(0, 1) = j12 / detJ;
		invMat(0, 2) = j13 / detJ;
		invMat(1, 0) = j21 / detJ;
		invMat(1, 1) = j22 / detJ;
		invMat(1, 2) = j23 / detJ;
		invMat(2, 0) = j31 / detJ;
		invMat(2, 1) = j32 / detJ;
		invMat(2, 2) = j33 / detJ;
		break;
	default:
		break;
	}
}