//Author		: Xiao Tuzi
//Date(Create)	: 08/13/2022
//Description	: this program is definition of of solution type variable


#include "Global/SolutionVar.h"

double* Ur;
double* Ur_t;
double* Ur_tt;

double* Us;
double* Us_t;
double* Us_tt;

std::complex<double>* CUs;
std::complex<double>* CUr;

double A[10]{};
double B[9]{};

int nEnfMotn = 0;
int iEnfMotn;

dMatrix dispVeloAcce;

dMatrix DVA;
zMatrix CDVA;

dMatrix Ubb;
dMatrix Ub;
zMatrix CUbb;
zMatrix CUb;

double alphad, betad, Gf, GEf, GfRatio, GEfRatio;
bool onlyFixedBnd;

// eigen values and vectors
double* eigVal;
double* eigVec;

// 
int iStep = 1;
int nStep = 1;