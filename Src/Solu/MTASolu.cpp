//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of Modal Transient Analysis Solu


#include "Solu/MTASolu.h"
#include "Global/GlobalVar.h"
#include "Keyword/KTIME.h"
#include "Keyword/KMODEXT.h"
#include "Global/SolutionVar.h"
#include "Global/Files.h"
#include "Initialize/Initialize.h"
#include "Initialize/ElemList.h"
#include "Initialize/LoadsList.h"
#include "Initialize/OutputList.h"
#include "Output/outputGlobalMatrix.h"
#include "MathSolver/MathSolver.h"
#include "Solu/Tools/calDamp.h"
#include "Solu/Tools/calModalGD.h"
#include "Solu/Tools/getTimeIntegral.h"
#include "Solu/Tools/getDisVelAcc.h"
#include "Solu/Tools/getNewAcce.h"

void MTASolu::initSolu() {
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

void MTASolu::execSolu() {
	// solution parameter
	int nMode0 = MODEXT->nModes;	//desired mode number
	double Emin = pow((2.0 * PI * MODEXT->MinFreq), 2);
	double Emax = pow((2.0 * PI * MODEXT->MaxFreq), 2);
	int nMode = nMode0;		// actural mode number

	double dt = TIME->TimeStep;
	double StartTime = TIME->StartTime;
	double EndTime = TIME->EndTime;
	int nTS = int((EndTime - StartTime) / dt);		// total time step
	nStep = nTS;

	// calculate Element assembly matrix
	std::cout << "global matrix is assembling..." << "\n";
	logfile << "global matrix is assembling..." << "\n";
	int iET;
	for (int i = 1; i <= nElem; i++) {
		iET = ELEM[i].ETypeID;
		ElemPtr[iET].P->calElemMat(i);	//calculate and assemble element matrix
	}

	outputGlobalMatrix();	// output global matrix information
	getTimeIntegral(dt, A, B);	// form time integral coefficient

	// solve eigen equations GK * X = ¦Ë * GM * X
	eigVal = new double[nMode0]();			// eigen values
	eigVec = new double[nMode0 * nEqu]();	// eigen vectors, eigVec = X.T
	std::cout << "modal solver is running..." << "\n";
	logfile << "modal solver is running..." << "\n";
	MS.eigenSolve(nMode0, Emin, Emax, eigVal, eigVec, nMode);

	// form coefficient matrix in modal space
	int mNNZ = nMode * nMode;		// NNZ in modal space
	double* mGK0 = new double[mNNZ]();
	double* mGK = new double[mNNZ]();	// GK in modal space
	double* mGM = new double[mNNZ]();	// GM in modal space
	double* mGD = new double[mNNZ]();	// GD in modal space
	double* mRHS0 = new double[nMode]();
	double* mRHS = new double[nMode]();	// RHS in modal space
	double* mU = new double[nMode]();	// U in modal space

	// mGD = X.T * GD * X, only for material damping
	calModalGD(eigVec, nMode, mGD);
	
	// get modal damping ksi
	double* ksi = new double[mNNZ]();		// modal damping
	calDamp(alphad, betad, GfRatio, GEfRatio, eigVal, nMode, ksi);
	for (int i = 0; i < mNNZ; i++){
		ksi[i] = 2.0 * sqrt(mGK[i]) * ksi[i];
	}

	// get mGK, mGM, mGD(overall)
	for (int i = 0; i < nMode; i++) {
		mGK[i * nMode + i] = eigVal[i];
		mGM[i * nMode + i] = 1.0;
	}
	for (int i = 0; i < mNNZ; i++){
			mGD[i] = alphad * mGM[i] + betad * mGK[i] + GfRatio * mGK[i] + GEfRatio * mGD[i] + ksi[i];
	}
	delete[] ksi;

	// form effective stiffness matrix in modal space
	memcpy(mGK0, mGK, mNNZ * sizeof(double));
	for (int i = 0; i < mNNZ; i++) {
		mGK[i] = A[6] * mGK0[i] + A[0] * mGM[i] + A[1] * mGD[i];
	}

	DVA.setSize(3, nEqu);
	dMatrix mDVA(3, nMode);	// DVA in modal space
	double* mTARHS = new double[nMode]();	// TARHS in modal space
	double* vLast = new double[nEqu]();

	// set initial condition for MTA
	// setInitCond(DVA);	// no initial condition in MTA

	std::cout << "loads condition is doing..." << "\n";
	logfile << "loads condition is doing..." << "\n";
 	LoadsPtr->init();
	// initialize history/general output
	GeneOutPtr->init();
	HistOutPtr->init();

	// time stepping
	double tNow = StartTime;
	LoadsPtr->formRHS(tNow);
	MS.dgemv(eigVec, nMode, nEqu, RHS, mRHS0);		// mTARHS = eigVec * RHS
	std::cout << "perform time stepping..." << "\n";
	logfile << "perform time stepping..." << "\n";

	for (int iTS = 1; iTS <= nTS; iTS++) {
		iStep = iTS;
		tNow += dt;
		printf("Load Step = %-6d      Time =%11.4e        Time Inc =%11.4e\n", iTS, tNow, dt);
		logfile << "Load Step = " << iTS << "  Time = " << tNow << " Time Inc = " << dt << "\n";

		// form RHS of DTA
		LoadsPtr->formRHS(tNow);
		MS.dgemv(eigVec, nMode, nEqu, RHS, mTARHS);	
		for (int i = 0; i < nMode; i++) {
			mRHS[i] = A[7] * mTARHS[i] + A[8] * mRHS0[i];
			mRHS0[i] = A[9] * mDVA(0, i);			// mRHS0 is regarded as temporary}
		}
		MS.dgemv(mGK0, nMode, nMode, mRHS0, mRHS, "N", 1.0, 1.0);
		for (int i = 0; i < nMode; i++) {
			mRHS0[i] = A[0] * mDVA(0, i) + A[2] * mDVA(1, i) + A[3] * mDVA(2, i);
		}
		MS.dgemv(mGM, nMode, nMode, mRHS0, mRHS, "N", 1.0, 1.0);
		for (int i = 0; i < nMode; i++) {
			mRHS0[i] = A[1] * mDVA(0, i) + A[4] * mDVA(1, i) + A[5] * mDVA(2, i);
		}
		MS.dgemv(mGD, nMode, nMode, mRHS0, mRHS, "N", 1.0, 1.0);

		memcpy(mRHS0, mTARHS, nMode * sizeof(double));	// mRHS0 is mTARHS at previous step

		// solve mGK * mU = mRHS, to get mU
		MS.dgesv(mGK, nMode, mU, mRHS);

		// get new mDVA from mU
		getDisVelAcc(mU, B, mDVA);

		// get DVA from mDVA, DVA = mDVA * eigVec 
		MS.dgemm(mDVA.datas(), eigVec, DVA.datas(), 3, nMode, nEqu);

		// get DVA based Us
		for (int i = 0; i < nEqu; i++){
			DVA(0, i) = DVA(0, i) + Us[i];
			DVA(1, i) = DVA(1, i) + Us_t[i];
			DVA(2, i) = DVA(2, i) + Us_tt[i];
		}

		// get new acceleration(DVA is changed)
		getNewAcce(dt, DVA, vLast);

		//execute history/general output
		GeneOutPtr->exec(tNow);
		HistOutPtr->exec(tNow);

		// restore DVA and get velocity of Last
		memcpy(&DVA(2, 0), vLast, nEqu * sizeof(double));
		memcpy(vLast, &DVA(1, 0), nEqu * sizeof(double));

	}	// end time stepping

	// finish history/general output
	GeneOutPtr->fini();
	HistOutPtr->fini();

	LoadsPtr->fini();

	// release new memory
	delete[] eigVal;
	delete[] eigVec;

	delete[] mGK0;
	delete[] mGK;
	delete[] mGM;
	delete[] mGD;
	delete[] mRHS0;
	delete[] mRHS;
	delete[] mU;

	DVA.destroy();
	mDVA.destroy();
	delete[] mTARHS;
	delete[] vLast;
}

void MTASolu::finiSolu() {
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