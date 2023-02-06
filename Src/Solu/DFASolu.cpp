//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of Direct Frequency Analysis Solu


#include "Solu/DFASolu.h"
#include "Global/GlobalVar.h"
#include "Keyword/KFREQ.h"
#include "Global/SolutionVar.h"
#include "Global/Files.h"
#include "Initialize/Initialize.h"
#include "Initialize/ElemList.h"
#include "Initialize/LoadsList.h"
#include "Initialize/OutputList.h"
#include "Output/outputGlobalMatrix.h"
#include "MathSolver/MathSolver.h"
#include "Solu/Tools/calDamp.h"

void DFASolu::initSolu() {
	Initialize initialize;
	initialize.exec();

	GK = new double[NNZ]();		// initialize global stiffness matrix
	GK12 = new double[NNZ12]();	// initialize global stiffness matrix
	GK22 = new double[NNZ22]();	// initialize global stiffness matrix
	GM = new double[NNZ]();		// initialize global mass matrix
	GM12 = new double[NNZ12](); // initialize global mass matrix
	GM22 = new double[NNZ22](); // initialize global mass matrix
	GD = new double[NNZ]();		// initialize global damp matrix
	GD12 = new double[NNZ12](); // initialize global damp matrix
	GD22 = new double[NNZ22](); // initialize global damp matrix

	CGK = new std::complex<double>[NNZ]();
}

void DFASolu::execSolu() {
	// solution parameter
	double df = FREQ->FreqStep;
	double StartFreq = FREQ->StartFreq;
	double EndFreq = FREQ->EndFreq;
	int nFS = int((EndFreq - StartFreq) / df);		// total frequency step
	nStep = nFS;

	// Calculate Element assembly matrix
	std::cout << "global matrix is assembling..." << "\n";
	logfile << "global matrix is assembling..." << "\n";
	int iET;
	for (int i = 1; i <= nElem; i++) {
		iET = ELEM[i].ETypeID;
		ElemPtr[iET].P->calElemMat(i);	//calculate and assemble element matrix
	}

	outputGlobalMatrix();	// output global matrix information
	calDamp(alphad, betad, Gf, GEf); // calculate damping parameter 
	
	CDVA.setSize(3, nEqu);

	// boundary condition
	std::cout << "loads condition is doing..." << "\n";
	logfile << "loads condition is doing..." << "\n";
	LoadsPtr->init();

	// initialize history/general output
	GeneOutPtr->init();
	HistOutPtr->init();

	// frequency stepping
	double fNow = StartFreq;
	double wNow;
	std::cout << "perform frequency stepping..." << "\n";
	logfile << "perform frequency stepping..." << "\n";

	for (int iFS = 1; iFS <= nFS; iFS++) {
		iStep = iFS;
		fNow += df;
		printf("Load Step = %-6d      Freq =%11.4e        Freq Inc =%11.4e\n", iFS, fNow, df);
		logfile << "Load Step = " << iFS << "  Freq = " << fNow << " Freq Inc = " << df << "\n";

		// effective stiffness matrix
		wNow = 2.0 * PI * fNow;
		for (int i = 0; i < NNZ; i++) {
			CGK[i].real(GK[i] - wNow * wNow * GM[i]);
			CGK[i].imag(wNow*(alphad * GM[i] + betad * GK[i]) + Gf * GK[i] + GEf * GD[i]);
		}

		// form CRHS of DFA
		LoadsPtr->formRHS(fNow);

		// solve complex linear algebra equation CGK * CU = CRHS, to get CU
		MS.zDirectSolve();

		// get CDVA from CU
		std::complex<double> temp = { 0.0, wNow };
		for (int i = 0; i < nEqu; i++){
			CDVA(0, i) = CU[i];
			CDVA(1, i) = temp * CDVA(0, i);
			CDVA(2, i) = temp * CDVA(1, i);
		}

		//execute history/general output
		GeneOutPtr->exec(fNow);
		HistOutPtr->exec(fNow);

	}	// end frequency step

	// finish history/general output
	GeneOutPtr->fini();
	HistOutPtr->fini();

	LoadsPtr->fini();

	// release new memory
	CDVA.destroy();
}

void DFASolu::finiSolu() {
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
	delete[] GD;
	delete[] GD12;
	delete[] GD22;

	delete[] CGK;
}