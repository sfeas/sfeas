//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is definition of Loads for DFASolu


#pragma once
#include "Loads/DFALoads.h"
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

void DFALoads::init() {
	CRHS = new std::complex<double>[nEqu]();
	CU = new std::complex<double>[nEqu]();
	CUr = new std::complex<double>[nEqu2]();

	getFixedBnd();
}

void DFALoads::formRHS(const double fNow) {
	memset(CRHS, 0, nEqu * sizeof(std::complex<double>));
	memset(CUr, 0, nEqu2 * sizeof(std::complex<double>));

	int NSID, DofID, CID1, CID2, NumNode, NID, IEQ;
	double forceValue1, forceValue2, dispValue1, dispValue2;
	double veloValue1, veloValue2, acceValue1, acceValue2;
	double w = 2.0 * PI * fNow;
	std::complex<double> temp0 = { 0.0, -1.0 / w };
	std::complex<double> temp1 = { -1.0 / w / w, 0.0 };
	std::complex<double> temp;

	// Force Loads(*FORCE)
	for (int i = 0; i < nFORCE; i++) {
		NSID = FORCE[i].NSetsID;
		DofID = FORCE[i].DofID;
		forceValue1 = FORCE[i].Value1;
		forceValue2 = FORCE[i].Value1;
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
		// Disp Loads(*DISP), get CUr
		for (int i = 0; i < nDISP; i++) {
			NSID = DISP[i].NSetsID;
			DofID = DISP[i].DofID;
			dispValue1 = DISP[i].Value1;
			dispValue2 = DISP[i].Value2;
			CID1 = DISP[i].CurveID1;
			CID2 = DISP[i].CurveID2;
			NumNode = NSETS[NSID].nSetsNode;
			if (CID1 > 0) { CURVE[CID1].GetValue(fNow, dispValue1); }
			if (CID2 > 0) { CURVE[CID2].GetValue(fNow, dispValue2); }

			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
				CUr[IEQ - 1] += {dispValue1, dispValue2};
			}
		}

		// Velo Loads(*VELO), get CUr
		for (int i = 0; i < nVELO; i++) {
			NSID = VELO[i].NSetsID;
			DofID = VELO[i].DofID;
			veloValue1 = VELO[i].Value1;
			veloValue2 = VELO[i].Value2;
			CID1 = VELO[i].CurveID1;
			CID2 = VELO[i].CurveID2;
			NumNode = NSETS[NSID].nSetsNode;
			if (CID1 > 0) { CURVE[CID1].GetValue(fNow, veloValue1); }
			if (CID2 > 0) { CURVE[CID2].GetValue(fNow, veloValue2); }

			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
				temp = { veloValue1, veloValue2 };
				CUr[IEQ - 1] += temp * temp0;
			}
		}

		// Acce Loads(*ACCE), get CUr
		for (int i = 0; i < nACCE; i++) {
			NSID = ACCE[i].NSetsID;
			DofID = ACCE[i].DofID;
			acceValue1 = ACCE[i].Value1;
			acceValue2 = ACCE[i].Value2;
			CID1 = ACCE[i].CurveID1;
			CID2 = ACCE[i].CurveID2;
			NumNode = NSETS[NSID].nSetsNode;
			if (CID1 > 0) { CURVE[CID1].GetValue(fNow, acceValue1); }
			if (CID2 > 0) { CURVE[CID2].GetValue(fNow, acceValue2); }

			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
				temp = { acceValue1, acceValue2 };
				CUr[IEQ - 1] += temp * temp1;
			}
		}

		// calculate RHS
		// F = F - (-w**2*M12 + iwC12 + K12)*Ur
		std::complex<double>* CGK12 = new std::complex<double>[NNZ12]();
		for (int i = 0; i < NNZ12; i++) {
			CGK12[i].real(GK12[i] - w * w * GM12[i]);
			CGK12[i].imag(w * (alphad * GM12[i] + betad * GK12[i]) + Gf * GK12[i] + GEf * GD12[i]);
		}
		std::complex<double> minOne = { -1.0, 0.0 };
		std::complex<double> One = { 1.0, 0.0 };
		MS.zcsrmv("F", CGK12, NNZ12, IROW12, JCOL12, nEqu, nEqu2, CUr, CRHS, minOne, One);

		delete[] CGK12;
	}
}

void DFALoads::fini() {
	delete[] CRHS;
	delete[] CU;
	delete[] CUr;
}