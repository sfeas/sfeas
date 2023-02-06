//Author		: Xiao Tuzi
//Date(Create)	: 08/13/2022
//Description	: this program is definition of global variable

#include "Global/GlobalVar.h"

/*----------------------global variables and parameters----------------------*/
std::string InputFileName; // input file name
std::string CurrentDirectory; // current directory

double *GK;			// global stiffness matrix
double *GK12;		// global stiffness matrix
double *GK22;		// global stiffness matrix
double *GD;			// global damping matrix
double *GD12;		// global damping matrix
double *GD22;		// global damping matrix
double *GM;			// global mass matrix
double *GM12;		// global mass matrix
double *GM22;		// global mass matrix
double *RHS;		// right hand side
double *U;			// displacement vector

std::complex<double>* CGK;	// global stiffness matrix in complex
std::complex<double>* CRHS;	// right hand side in complex
std::complex<double>* CU;	// displacement vector in complex

int NNZ;			// non-zeros of GK(CSR)
int NNZ12;			// non-zeros of GK(CSR)
int NNZ22;			// non-zeros of GK(CSR)
int *IROW;			// row index of GK(CSR)
int *IROW12;		// row index of GK(CSR)
int *IROW22;		// row index of GK(CSR)
int *JCOL;			// col index of GK(CSR)
int *JCOL12;		// col index of GK(CSR)
int *JCOL22;		// col index of GK(CSR)
int nEqu;			// total number of Equations of freedom dof
int nEqu2;			// total number of Equations of restrain dof
int nDof;			// total number of Dof

double PI = 3.1415926535898;	// parameter PI
