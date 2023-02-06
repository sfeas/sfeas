//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of MatIsoElastic


#include "Material/MatIsoElastic.h"
#include "Global/GlobalVar.h"
#include "Keyword/KMAT.h"

void MatIsoElastic::calMatD(int iMat, dMatrix& DMat) {
    // DMat(6, 6) for Iso elastic material
	double EX = MAT[iMat].EX;
	double MU = MAT[iMat].MU;

    double G = 0.5 * EX / (1.0 + MU);
    double lamda = EX * MU / ((1.0 + MU) * (1.0 - 2.0 * MU));
    double A = lamda + 2.0 * G;

    DMat(0, 0) = A;
    DMat(1, 0) = lamda;
    DMat(2, 0) = lamda;
    DMat(0, 1) = lamda;
    DMat(1, 1) = A;
    DMat(2, 1) = lamda;
    DMat(0, 2) = lamda;
    DMat(1, 2) = lamda;
    DMat(2, 2) = A;
    DMat(3, 3) = G;
    DMat(4, 4) = G;
    DMat(5, 5) = G;
}
