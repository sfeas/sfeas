//Author		: Xiao Tuzi
//Date(Create)	: 11/27/2022
//Description	: this program is declaration of solution type variable


#pragma once
#include "DataType/Matrix.h"

extern double* Ur;		// restrain displacement vector
extern double* Ur_t;	// restrain velocity vector
extern double* Ur_tt;	// restrain acceleration vector

extern double* Us;		// static displacement vector
extern double* Us_t;	// static velocity vector
extern double* Us_tt;	// static acceleration vector

extern std::complex<double>* CUr;	// restrain displacement vector in complex
extern std::complex<double>* CUs;	// restrain displacement vector in complex


extern double A[10];	// time integral coefficient A
extern double B[9];		// time integral coefficient B

extern int nEnfMotn;
extern int iEnfMotn;

// dispVeloAcce(nEnfMotn, 3) is to calculate the relationship of enforced disp, velo, acce
extern dMatrix dispVeloAcce;

// DVA(3, nEqu) is to save disp, velo, acce
extern dMatrix DVA;
extern zMatrix CDVA;


// enforced motion in MTA/MFA
// Ubb by solving GK * Ubb = -GK12 * Ub
// Ubb:	Us is linear combination of Ubb
// Ub:	basic unit vector by *DISP, *VELO, *ACCE
extern dMatrix Ubb;
extern dMatrix Ub;
extern zMatrix CUbb;
extern zMatrix CUb;

// damping parameter
extern double alphad, betad, Gf, GEf, GfRatio, GEfRatio;
extern bool onlyFixedBnd;

// eigen values and vectors
extern double* eigVal;
extern double* eigVec;

// Loop step
extern int iStep;
extern int nStep;