//Author		: Xiao Tuzi
//Date(Create)	: 09/09/2022
//Description	: this program is calculating new DisVelAcc in time integral algrithm of TASolu


#include "Solu/Tools/getDisVelAcc.h"

void getDisVelAcc(double* U, const double* B, dMatrix& DVA) {
	double disp, velo, acce;
	// get DisVelAcc for different time integral algrithm
	for (int i = 0; i < DVA.cols(); i++) {
		acce = B[0] * (U[i] - DVA(0, i)) + B[1] * DVA(1, i) + B[2] * DVA(2, i);
		velo = B[3] * (U[i] - DVA(0, i)) + B[4] * DVA(1, i) + B[5] * DVA(2, i);
		disp = DVA(0, i) + B[6] * (U[i] - DVA(0, i)) + B[7] * DVA(1, i) + B[8] * DVA(2, i);

		DVA(0, i) = disp;
		DVA(1, i) = velo;
		DVA(2, i) = acce;
	}
}