//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of Modal Analysis Solu


#include "Solu/MOASolu.h"
#include "Global/GlobalVar.h"
#include "Keyword/KMODEXT.h"
#include "Global/SolutionVar.h"
#include "Global/Files.h"
#include "Initialize/Initialize.h"
#include "Initialize/ElemList.h"
#include "Initialize/LoadsList.h"
#include "Initialize/OutputList.h"
#include "MathSolver/MathSolver.h"
#include "Output/outputGlobalMatrix.h"

void MOASolu::initSolu() {
	Initialize initialize;
	initialize.exec();

	GK = new double[NNZ]();		// initialize global stiffness matrix
	GK12 = new double[NNZ12]();	// initialize global stiffness matrix
	GK22 = new double[NNZ22]();	// initialize global stiffness matrix
	GM = new double[NNZ]();		// initialize global mass matrix
	GM12 = new double[NNZ12](); // initialize global mass matrix
	GM22 = new double[NNZ22](); // initialize global mass matrix
}

void MOASolu::execSolu() {
	// Calculate Element assembly matrix
	std::cout << "global matrix is assembling..." << "\n";
	logfile << "global matrix is assembling..." << "\n";
	int iET;
	for (int i = 1; i <= nElem; i++) {
		iET = ELEM[i].ETypeID;
		ElemPtr[iET].P->calElemMat(i);	//calculate and assemble element matrix
	}

	outputGlobalMatrix();	// output global matrix information

	// Modal solve parameter
	int nMode0 = MODEXT->nModes;
	double Emin = pow((2.0 * PI * MODEXT->MinFreq), 2);
	double Emax = pow((2.0 * PI * MODEXT->MaxFreq), 2);
	int nMode = nMode0;		// actural mode number

	// Solve eigen equations GK * X = ¦Ë * GM * X
	eigVal = new double[nMode0]();			// eigen values
	eigVec = new double[nEqu * nMode0]();	// eigen vectors
	std::cout << "modal solver is running..." << "\n";
	logfile << "modal solver is running..." << "\n";
	MS.eigenSolve(nMode0, Emin, Emax, eigVal, eigVec, nMode);

	// Output results
	std::cout << "result is outputing..." << "\n";
	logfile << "result is outputing..." << "\n";
	
	// boundary condition
	std::cout << "loads condition is doing..." << "\n";
	logfile << "loads condition is doing..." << "\n";
	LoadsPtr->init();
	LoadsPtr->formRHS();

	GeneOutPtr->init();
	for (int i = 0; i < nMode; i++) {
		memcpy(U, &eigVec[nEqu * i], nEqu * sizeof(double));
		GeneOutPtr->exec(sqrt(eigVal[i]) / 2.0 / PI);
	}

	GeneOutPtr->fini();
	LoadsPtr->fini();

	// release memory
	delete[] eigVal;
	delete[] eigVec;
}

void MOASolu::finiSolu() {
	// delete new storage in initSolu
	delete[] IROW;
	delete[] IROW12;
	delete[] IROW22;
	delete[] JCOL;
	delete[] JCOL12;
	delete[] JCOL22;

	delete[] GK;
	delete[] GK12;
	delete[] GK22;
	delete[] GM;
	delete[] GM12;
	delete[] GM22;
}