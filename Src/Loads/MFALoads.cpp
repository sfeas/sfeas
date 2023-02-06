//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is definition of Loads for MFASolu


#pragma once
#include "Loads/MFALoads.h"
#include "Global/GlobalVar.h"
#include "Keyword/KFORCE.h"
#include "Keyword/KDISP.h"
#include "Keyword/KVELO.h"
#include "Keyword/KACCE.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "MathSolver/MathSolver.h"
#include "Global/SolutionVar.h"
#include "Loads/Tools/getFixedBnd.h"

void MFALoads::init() {
	RHS = new double[nEqu]();
	CRHS = new std::complex<double>[nEqu]();
	CU = new std::complex<double>[nEqu]();
	CUr = new std::complex<double>[nEqu2]();
	CUs = new std::complex<double>[nEqu]();

	getFixedBnd();

	nEnfMotn = nDISP + nVELO + nACCE;
	Ubb.setSize(nEnfMotn, nEqu);
	Ub.setSize(nEnfMotn, nEqu2);

	// get Ub from DISP, VELO, ACCE
	int NSID, DofID, NumNode, NID, IEQ;
	int count = -1;

	// from DISP
	for (int i = 0; i < nDISP; i++) {
		count++;
		NSID = DISP[i].NSetsID;
		DofID = DISP[i].DofID;
		NumNode = NSETS[NSID].nSetsNode;
		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
			Ub(count, IEQ - 1) = 1.0;
		}
	}

	// from VELO
	for (int i = 0; i < nVELO; i++) {
		count++;
		NSID = VELO[i].NSetsID;
		DofID = VELO[i].DofID;
		NumNode = NSETS[NSID].nSetsNode;
		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
			Ub(count, IEQ - 1) = 1.0;
		}
	}

	// from ACCE
	for (int i = 0; i < nACCE; i++) {
		count++;
		NSID = ACCE[i].NSetsID;
		DofID = ACCE[i].DofID;
		NumNode = NSETS[NSID].nSetsNode;
		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
			Ub(count, IEQ - 1) = 1.0;
		}
	}

	if (!onlyFixedBnd) {
		// get Ubb by solving GK * Ubb = -GK12 * Ub
		MS.decompose();
		for (int i = 0; i < nEnfMotn; i++) {
			MS.dcsrmv("F", GK12, IROW12, JCOL12, nEqu, nEqu2, &Ub(i, 0), RHS, -1.0, 0.0);
			MS.substitute(nEqu, RHS, &Ubb(i, 0));
		}
		MS.release();
	}
}

void MFALoads::formRHS(const double fNow) {
	memset(CRHS, 0, nEqu * sizeof(std::complex<double>));
	memset(CUs, 0, nEqu * sizeof(std::complex<double>));
	memset(CUr, 0, nEqu2 * sizeof(std::complex<double>));
	iEnfMotn = -1;

	int NSID, DofID, CID1, CID2, NumNode, NID, IEQ;
	double forceValue1, forceValue2, dispValue1, dispValue2;
	double veloValue1, veloValue2, acceValue1, acceValue2;
	double w = 2.0 * PI * fNow;
	std::complex<double> temp0 = { 0.0, -1.0 / w };
	std::complex<double> temp1 = { -1.0 / w / w, 0.0 };

	// Force Loads(*FORCE)
	for (int i = 0; i < nFORCE; i++) {
		NSID = FORCE[i].NSetsID;
		DofID = FORCE[i].DofID;
		forceValue1 = FORCE[i].Value1;
		forceValue2 = FORCE[i].Value2;
		CID1 = FORCE[i].CurveID1;
		CID2 = FORCE[i].CurveID2;
		NumNode = NSETS[NSID].nSetsNode;
		if (CID1 > 0) { CURVE[CID1].GetValue(fNow, forceValue1); }
		if (CID2 > 0) { CURVE[CID2].GetValue(fNow, forceValue2); }

		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			IEQ = NStat[TNode[NID]].DOF[DofID - 1];
			if (IEQ <= nEqu) { CRHS[IEQ - 1] += {forceValue1, forceValue2}; }
		}
	}

	if (!onlyFixedBnd) {
		// Disp Loads(*DISP), get CUr, CUs
		for (int j = 0; j < nDISP; j++) {
			iEnfMotn++;
			CID1 = DISP[j].CurveID1;
			CID2 = DISP[j].CurveID2;
			dispValue1 = DISP[j].Value1;
			dispValue2 = DISP[j].Value2;
			if (CID1 > 0) { CURVE[CID1].GetValue(fNow, dispValue1); }
			if (CID2 > 0) { CURVE[CID2].GetValue(fNow, dispValue2); }

			for (int i = 0; i < nEqu2; i++) {
				CUr[i].real(CUr[i].real() + dispValue1 * Ub(iEnfMotn, i));
				CUr[i].imag(CUr[i].imag() + dispValue2 * Ub(iEnfMotn, i));
			}
			for (int i = 0; i < nEqu; i++) {
				CUs[i].real(CUs[i].real() + dispValue1 * Ubb(iEnfMotn, i));
				CUs[i].imag(CUs[i].imag() + dispValue2 * Ubb(iEnfMotn, i));
			}
		}

		// Velo Loads(*VELO), get CUr, CUs
		for (int j = 0; j < nVELO; j++) {
			iEnfMotn++;
			CID1 = VELO[j].CurveID1;
			CID2 = VELO[j].CurveID2;
			veloValue1 = VELO[j].Value1;
			veloValue2 = VELO[j].Value2;
			if (CID1 > 0) { CURVE[CID1].GetValue(fNow, veloValue1); }
			if (CID2 > 0) { CURVE[CID2].GetValue(fNow, veloValue2); }

			for (int i = 0; i < nEqu2; i++) {
				CUr[i].real(CUr[i].real() + veloValue1 * Ub(iEnfMotn, i));
				CUr[i].imag(CUr[i].imag() + veloValue2 * Ub(iEnfMotn, i));
				CUr[i] *= temp0;
			}
			for (int i = 0; i < nEqu; i++) {
				CUs[i].real(CUs[i].real() + veloValue1 * Ubb(iEnfMotn, i));
				CUs[i].imag(CUs[i].imag() + veloValue2 * Ubb(iEnfMotn, i));
				CUs[i] *= temp0;
			}
		}

		// Acce Loads(*ACCE), get CUr
		for (int j = 0; j < nACCE; j++) {
			iEnfMotn++;
			CID1 = ACCE[j].CurveID1;
			CID2 = ACCE[j].CurveID2;
			acceValue1 = ACCE[j].Value1;
			acceValue2 = ACCE[j].Value2;
			if (CID1 > 0) { CURVE[CID1].GetValue(fNow, acceValue1); }
			if (CID2 > 0) { CURVE[CID2].GetValue(fNow, acceValue2); }

			for (int i = 0; i < nEqu2; i++) {
				CUr[i].real(CUr[i].real() + acceValue1 * Ub(iEnfMotn, i));
				CUr[i].imag(CUr[i].imag() + acceValue2 * Ub(iEnfMotn, i));
				CUr[i] *= temp1;
			}
			for (int i = 0; i < nEqu; i++) {
				CUs[i].real(CUs[i].real() + acceValue1 * Ubb(iEnfMotn, i));
				CUs[i].imag(CUs[i].imag() + acceValue2 * Ubb(iEnfMotn, i));
				CUs[i] *= temp1;
			}
		}

		// calculate RHS
		// F = F - (-w**2*M11 + iwC11)Us - (-w**2*M12 + iwC12)*Ur

		double w = 2.0 * PI * fNow;
		std::complex<double> minOne = { -1.0, 0.0 };
		std::complex<double> One = { 1.0, 0.0 };
		CGK = new std::complex<double>[NNZ]();
		std::complex<double>* CGK12 = new std::complex<double>[NNZ12]();

		for (int i = 0; i < NNZ; i++) {
			CGK[i].real(-w * w * GM[i]);
			CGK[i].imag(w * (alphad * GM[i] + betad * GK[i]) + Gf * GK[i] + GEf * GD[i]);
		}
		MS.zcsrmv("U", CGK, NNZ, IROW, JCOL, nEqu, nEqu, CUs, CRHS, minOne, One);

		for (int i = 0; i < NNZ12; i++) {
			CGK12[i].real(-w * w * GM12[i]);
			CGK12[i].imag(w * (alphad * GM12[i] + betad * GK12[i]) + Gf * GK12[i] + GEf * GD12[i]);
		}
		MS.zcsrmv("F", CGK12, NNZ12, IROW12, JCOL12, nEqu, nEqu2, CUr, CRHS, minOne, One);

		delete[] CGK;
		delete[] CGK12;
	}
}

void MFALoads::fini() {
	delete[] RHS;
	delete[] CRHS;
	delete[] CU;
	delete[] CUr;
	delete[] CUs;

	Ubb.destroy();
	Ub.destroy();
}