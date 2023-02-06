//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of Direct Transient Analysis Solu


#include "Solu/DTASolu.h"
#include "Global/GlobalVar.h"
#include "Keyword/KTIME.h"
#include "Global/SolutionVar.h"
#include "Global/Files.h"
#include "Initialize/Initialize.h"
#include "Initialize/ElemList.h"
#include "Initialize/LoadsList.h"
#include "Initialize/OutputList.h"
#include "Output/outputGlobalMatrix.h"
#include "MathSolver/MathSolver.h"
#include "Solu/Tools/calDamp.h"
#include "Solu/Tools/getTimeIntegral.h"
#include "Solu/Tools/getDisVelAcc.h"
#include "Solu/Tools/getNewAcce.h"

void DTASolu::initSolu() {
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
}

void DTASolu::execSolu() {
	// solution parameter
	double dt = TIME->TimeStep;
	double StartTime = TIME->StartTime;
	double EndTime = TIME->EndTime;
	int nTS = int((EndTime - StartTime) / dt);		// total time step
	nStep = nTS;

	// calculate element assembly matrix
	std::cout << "global matrix is assembling..." << "\n";
	logfile << "global matrix is assembling..." << "\n";
	int iET;
	for (int i = 1; i <= nElem; i++) {
		iET = ELEM[i].ETypeID;
		ElemPtr[iET].P->calElemMat(i);	//calculate and assemble element matrix
	}

	outputGlobalMatrix();	// output global matrix information
	calDamp(alphad, betad, GfRatio, GEfRatio);	// calculate damping parameter 
	getTimeIntegral(dt, A, B);	// form time integral coefficient

	// form GD
	for (int i = 0; i < NNZ; i++) {
		GD[i] = alphad * GM[i] + betad * GK[i] + GfRatio * GK[i] + GEfRatio * GD[i];
	}

	// form effective stiffness matrix, and decompose
	std::cout << "effective stiffness matrix is decomposing..." << "\n";
	logfile << "effective stiffness matrix is decomposing..." << "\n";
	double* GK0 = new double[NNZ]();
	memcpy(GK0, GK, NNZ * sizeof(double));
	for (int i = 0; i < NNZ; i++) {
		GK[i] = A[6] * GK0[i] + A[0] * GM[i] + A[1] * GD[i];
	}
	MS.decompose();

	// DVA is to save disp, velo, acce
	DVA.setSize(3, nEqu);
	double* RHS0 = new double[nEqu]();
	double* temp = new double[nEqu]();
	double* vLast = new double[nEqu]();

	// set initial condition for DTA
	// setInitCond(DVA);

	// boundary condition
	std::cout << "loads condition is doing..." << "\n";
	logfile << "loads condition is doing..." << "\n";
	LoadsPtr->init();

	// initialize history/general output
	GeneOutPtr->init();
	HistOutPtr->init();

	// time stepping
	double tNow = StartTime;
	LoadsPtr->formRHS(tNow);
	memcpy(RHS0, RHS, nEqu * sizeof(double));

	std::cout << "perform time stepping..." << "\n";
	logfile << "perform time stepping..." << "\n";

	for (int iTS = 1; iTS <= nTS; iTS++) {
		iStep = iTS;
		tNow += dt;
		printf("Load Step = %-6d      Time =%11.4e        Time Inc =%11.4e\n", iTS, tNow, dt);
		logfile << "Load Step = " << iTS << "  Time = " << tNow << " Time Inc = " << dt << "\n";

		// form RHS of DTA
		LoadsPtr->formRHS(tNow);
		memcpy(temp, RHS, nEqu * sizeof(double));
		for (int i = 0; i < nEqu; i++) { 
			RHS[i] = A[7] * RHS[i] + A[8] * RHS0[i];
			RHS0[i] = A[9] * DVA(0, i);			// RHS0 is regarded as temporary
		}
		MS.dcsrmv("U", GK0, IROW, JCOL, nEqu, nEqu, RHS0, RHS, 1.0, 1.0);
		for (int i = 0; i < nEqu; i++) {
			RHS0[i] = A[0] * DVA(0, i) + A[2] * DVA(1, i) + A[3] * DVA(2, i);
		}
		MS.dcsrmv("U", GM, IROW, JCOL, nEqu, nEqu, RHS0, RHS, 1.0, 1.0);
		for (int i = 0; i < nEqu; i++) {
			RHS0[i] = A[1] * DVA(0, i) + A[4] * DVA(1, i) + A[5] * DVA(2, i);
		}
		MS.dcsrmv("U", GD, IROW, JCOL, nEqu, nEqu, RHS0, RHS, 1.0, 1.0);

		memcpy(RHS0, temp, nEqu * sizeof(double));	// RHS0 is RHS at previous step

		// back substitution to get U
		MS.substitute();

		// get new DVA from U
		getDisVelAcc(U, B, DVA);

		// get new acceleration(DVA is changed)
		getNewAcce(dt, DVA, vLast);

		//execute history/general output
		GeneOutPtr->exec(tNow);
		HistOutPtr->exec(tNow);

		// restore DVA and get velocity of Last
		memcpy(&DVA(2, 0), vLast, nEqu * sizeof(double));
		memcpy(vLast, &DVA(1, 0), nEqu * sizeof(double));

	}	// end time stepping

	// release Solver memory
	MS.release();

	// finish history/general output
	GeneOutPtr->fini();
	HistOutPtr->fini();

	LoadsPtr->fini();

	// release new memory
	delete[] GK0;
	DVA.destroy();
	delete[] RHS0;
	delete[] temp;
	delete[] vLast;
}

void DTASolu::finiSolu() {
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
}