//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of Modal Frequency Analysis Solu


#include "Solu/MFASolu.h"
#include "Global/GlobalVar.h"
#include "Keyword/KFREQ.h"
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

void MFASolu::initSolu() {
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

void MFASolu::execSolu() {
	// solution parameter
	int nMode0 = MODEXT->nModes;
	double Emin = pow((2.0 * PI * MODEXT->MinFreq), 2);
	double Emax = pow((2.0 * PI * MODEXT->MaxFreq), 2);
	int nMode = nMode0;		// actural mode number

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

	// solve eigen equations GK * X = ¦Ë * GM * X
	eigVal = new double[nMode0]();			// eigen values
	eigVec = new double[nMode0 * nEqu]();	// eigen vectors, eigVec = X.T
	std::cout << "modal solver is running..." << "\n";
	logfile << "modal solver is running..." << "\n";
	MS.eigenSolve(nMode0, Emin, Emax, eigVal, eigVec, nMode);

	// form coefficient matrix in modal space
	int mNNZ = nMode * nMode;		// NNZ in modal space
	double* mGK = new double[mNNZ]();	// GK in modal space
	double* mGM = new double[mNNZ]();	// GM in modal space
	double* mGD = new double[mNNZ]();	// GD in modal space
	std::complex<double>* mCGK = new std::complex<double>[mNNZ]();	// CGK in modal space
	std::complex<double>* mCRHS = new std::complex<double>[nMode]();	// CRHS in modal space
	std::complex<double>* mCU = new std::complex<double>[nMode]();		// CU in modal space
	
	// eigVec in complex
	std::complex<double>* CeigVec = new std::complex<double>[nMode * nEqu]();		
	for (int i = 0; i < nMode * nEqu; i++){
		CeigVec[i].real(eigVec[i]);
	}

	// mGD = X.T * GD * X, only for material damping
	calModalGD(eigVec, nMode, mGD);

	// get modal damping ksi;
	double* ksi = new double[mNNZ]();
	calDamp(alphad, betad, Gf, GEf, eigVal, nMode, ksi);
	for (int i = 0; i < mNNZ; i++) {
		ksi[i] = 2.0 * PI * sqrt(mGK[i]) * ksi[i];
	}

	// get mGK, mGM
	for (int i = 0; i < nMode; i++) {
		mGK[i * nMode + i] = eigVal[i];
		mGM[i * nMode + i] = 1.0;
	}

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
		for (int i = 0; i < mNNZ; i++) {
			mCGK[i].real(mGK[i] - wNow * wNow * mGM[i]);
			mCGK[i].imag(wNow * (alphad * mGM[i] + betad * mGK[i] + ksi[i]) + Gf * mGK[i] + GEf * mGD[i]);
		}

		// form CRHS of MFA
		LoadsPtr->formRHS(fNow);

		// get mCRHS from CRHS, mCRHS = X.T * CRHS
		MS.zgemv(CeigVec, nMode, nEqu, CRHS, mCRHS);	

		// solve complex linear algebra equation mCGK * mU = mRHS, to get mU
		MS.zgesv(mCGK, nMode, mCU, mCRHS);

		// get CU from mCU, CU = X * mCU
		MS.zgemv(CeigVec, nEqu, nMode, mCU, CU, "T");

		// get DVA from CU
		std::complex<double> temp = { 0.0, wNow };
		for (int i = 0; i < nEqu; i++) {
			CDVA(0, i) = CU[i] + CUs[i];
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
	delete[] eigVal;
	delete[] eigVec;

	delete[] mGK;
	delete[] mGM;
	delete[] mGD;
	delete[] mCGK;
	delete[] mCRHS;
	delete[] mCU;

	delete[] ksi;
	CDVA.destroy();
}

void MFASolu::finiSolu() {
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