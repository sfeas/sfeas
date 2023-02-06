//Author		: Xiao Tuzi
//Date(Create)	: 09/16/2022
//Description	: this program is calculating damping matrix in modal space


#include "Solu/Tools/calModalGD.h"
#include "Global/GlobalVar.h"
#include "MathSolver/MathSolver.h"

void calModalGD(double* eigVec, int nMode, double* mGD) {
	// mGD = X.T * GD * X, X = eigVec.T
	// eigVec[nMode, nEqu], X[nEqu, nMode]
	bool GDisZero = true;

	for (int i = 0; i < NNZ; i++){
		if (GD[i] != 0.0) {
			GDisZero = false;
			break;
		}
	}

	if (!GDisZero) {
		double* temp = new double[nMode * nEqu]();		// temp[i, 1:nEqu] = GD * X[1:nEqu, i] 
		for (int i = 0; i < nMode; i++) {
			MS.dcsrmv("U", GD, IROW, JCOL, nEqu, nEqu, eigVec + i * nEqu, temp + i * nEqu);
		}
		MS.dgemm(eigVec, temp, mGD, nMode, nEqu, nMode, "N", "T");

		delete[] temp;
	}
}

