//Author		: Xiao Tuzi
//Date(Create)	: 08/27/2022
//Description	: this program is updating U0 to NStat[i].U


#include "Global/GlobalVar.h"
#include "Global/SolutionVar.h"
#include "Output/Tools/updateU.h"

void updateU(double* U0) {
	// update U0 to NStat[i].U
	int IEQ;
	for (int i = 1; i <= nNode; i++) {
		for (int j = 0; j < NStat[i].NDOFN; j++) {
			IEQ = NStat[i].DOF[j];
			if (IEQ <= nEqu) { 
				NStat[i].U[j] = U0[IEQ - 1];
			}
			else {
				NStat[i].U[j] = Ur[IEQ - 1 - nEqu];
			}
		}
	}
}

void updateU(dMatrix DVA) {
	// update DVA to NStat[i].U, NStat[i].V, NStat[i].A
	int IEQ;
	for (int i = 1; i <= nNode; i++) {
		for (int j = 0; j < NStat[i].NDOFN; j++) {
			IEQ = NStat[i].DOF[j];
			if (IEQ <= nEqu) { 
				NStat[i].U[j] = DVA(0, IEQ - 1);
				NStat[i].V[j] = DVA(1, IEQ - 1);
				NStat[i].A[j] = DVA(2, IEQ - 1);
			}
			else {
				NStat[i].U[j] = Ur[IEQ - 1 - nEqu];
				NStat[i].V[j] = Ur_t[IEQ - 1 - nEqu];
				NStat[i].A[j] = Ur_tt[IEQ - 1 - nEqu];
			}
		}
	}
}

void updateU(zMatrix CDVA, double fNow, const int label) {
	// update CDVA(complex) to NStat[i].U, NStat[i].V, NStat[i].A
	int IEQ;
	std::complex<double> temp = { 0.0, 2.0 * PI * fNow };

	for (int i = 1; i <= nNode; i++) {
		for (int j = 0; j < NStat[i].NDOFN; j++) {
			IEQ = NStat[i].DOF[j];
			if (IEQ <= nEqu) {
				// mag
				if (label == 0) {
					NStat[i].U[j] = abs(CDVA(0, IEQ - 1));
					NStat[i].V[j] = abs(CDVA(1, IEQ - 1));
					NStat[i].A[j] = abs(CDVA(2, IEQ - 1));
				}
				// real part
				else if (label == 1) {
					NStat[i].U[j] = CDVA(0, IEQ - 1).real();
					NStat[i].V[j] = CDVA(1, IEQ - 1).real();
					NStat[i].A[j] = CDVA(2, IEQ - 1).real();
				}
				// imag part
				else if (label == 2) {
					NStat[i].U[j] = CDVA(0, IEQ - 1).imag();
					NStat[i].V[j] = CDVA(1, IEQ - 1).imag();
					NStat[i].A[j] = CDVA(2, IEQ - 1).imag();
				}
			}
			else {
				if (label == 0) {
					NStat[i].U[j] = abs(CUr[IEQ - 1 - nEqu]);
					temp = temp * CUr[IEQ - 1 - nEqu];
					NStat[i].V[j] = abs(temp);
					temp = temp * temp;
					NStat[i].A[j] = abs(temp);
				}
				if (label == 1) {
					NStat[i].U[j] = CUr[IEQ - 1 - nEqu].real();
					temp = temp * CUr[IEQ - 1 - nEqu];
					NStat[i].V[j] = temp.real();
					temp = temp * temp;
					NStat[i].A[j] = temp.real();
				}
				if (label == 2) { 
					NStat[i].U[j] = CUr[IEQ - 1 - nEqu].imag(); 
					temp = temp * CUr[IEQ - 1 - nEqu];
					NStat[i].V[j] = temp.imag();
					temp = temp * temp;
					NStat[i].A[j] = temp.imag();
				}
			}
		}
	}
}