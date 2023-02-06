//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of Static Analysis Solu


#include "Solu/STASolu.h"
#include "Global/GlobalVar.h"
#include "Global/SolutionVar.h"
#include "Global/Files.h"
#include "Initialize/Initialize.h"
#include "Initialize/ElemList.h"
#include "Initialize/LoadsList.h"
#include "Initialize/OutputList.h"
#include "MathSolver/MathSolver.h"
#include "Output/outputGlobalMatrix.h"

void STASolu::initSolu() {
	Initialize initialize;
	initialize.exec();

	GK = new double[NNZ]();		// initialize global stiffness matrix
	GK12 = new double[NNZ12]();	// initialize global stiffness matrix
	GK22 = new double[NNZ22]();	// initialize global stiffness matrix
}

void STASolu::execSolu() {
	// Calculate Element assembly matrix
	std::cout << "global matrix is assembling..." << "\n";
	logfile << "global matrix is assembling..." << "\n";
	int iET;
	for (int i = 1; i <= nElem; i++) {
		iET = ELEM[i].ETypeID;
		ElemPtr[iET].P->calElemMat(i);	//calculate and assemble element matrix
	}
	
	outputGlobalMatrix();	// output global matrix information

	// Boundary Condition
	std::cout << "loads condition is doing..." << "\n";
	logfile << "loads condition is doing..." << "\n";

	LoadsPtr->init();
	LoadsPtr->formRHS();

	// Solve linear equations GK * U = RHS
	std::cout << "equation solver is running..." << "\n";
	logfile << "equation solver is running..." << "\n";
	//MS.decompose();
	//MS.substitute();
	//MS.release();
	MS.DirectSolve();

	// Output results
	std::cout << "result is outputing..." << "\n";
	logfile << "result is outputing..." << "\n";
	GeneOutPtr->init();
	GeneOutPtr->exec();
	GeneOutPtr->fini();

	LoadsPtr->fini();
}

void STASolu::finiSolu() {
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
}