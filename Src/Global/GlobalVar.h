//Author		: Xiao Tuzi
//Date(Create)	: 08/12/2022
//Description	: this program is declaration of global variable

#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <set>
#include <complex>

#include "Keyword/KNODE.h"
#include "Keyword/KELEM.h"

/*----------------------global variables and parameters----------------------*/
extern std::string InputFileName; // input file name
extern std::string CurrentDirectory; // current directory

extern double *GK;		// global stiffness matrix
extern double *GK12;	// global stiffness matrix
extern double *GK22;	// global stiffness matrix
extern double *GD;		// global damping matrix
extern double *GD12;	// global damping matrix
extern double *GD22;	// global damping matrix
extern double *GM;		// global mass matrix
extern double *GM12;	// global mass matrix
extern double *GM22;	// global mass matrix
extern double *RHS;		// right hand side
extern double *U;		// displacement vector

extern std::complex<double>* CGK;	// global stiffness matrix in complex
extern std::complex<double>* CRHS;	// right hand side in complex
extern std::complex<double>* CU;	// displacement vector in complex

extern int NNZ;			// non-zeros of GK(CSR)
extern int NNZ12;		// non-zeros of GK(CSR)
extern int NNZ22;		// non-zeros of GK(CSR)
extern int *IROW;		// row index of GK(CSR)
extern int *IROW12;		// row index of GK(CSR)
extern int *IROW22;		// row index of GK(CSR)
extern int *JCOL;		// col index of GK(CSR)
extern int *JCOL12;		// col index of GK(CSR)
extern int *JCOL22;		// col index of GK(CSR)
extern int nEqu;		// total number of Equations of freedom dof
extern int nEqu2;		// total number of Equations of restrain dof
extern int nDof;		// total number of Dof

extern double PI;		// parameter PI
