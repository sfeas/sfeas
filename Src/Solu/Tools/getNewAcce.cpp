//Author		: Xiao Tuzi
//Date(Create)	: 09/12/2022
//Description	: this program is calculating new Acce in time integral algrithm of TASolu


#include "Solu/Tools/getNewAcce.h"

void getNewAcce(double dt, dMatrix& DVA, double* temp) {
	
	// input:	temp is velocity of Last
	// output:	temp is oldAcce(DVA[2][i]), DVA[2][i] is changed to newAcce
	double vLast;
	for (int i = 0; i < DVA.cols(); i++) {
		vLast = temp[i];
		temp[i] = DVA(2, i);
		DVA(2, i) = (DVA(1, i) - vLast) / dt;
	}
}