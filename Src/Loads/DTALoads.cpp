//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is definition of Loads for DTASolu


#pragma once
#include "Loads/DTALoads.h"
#include "Global/GlobalVar.h"
#include "Keyword/KFORCE.h"
#include "Keyword/KDISP.h"
#include "Keyword/KVELO.h"
#include "Keyword/KACCE.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "MathSolver/MathSolver.h"
#include "Global/SolutionVar.h"
#include "Loads/Tools/getDVAValue.h"
#include "Loads/Tools/getFixedBnd.h"

void DTALoads::init() {
	RHS = new double[nEqu]();
	U = new double[nEqu]();
	Ur = new double[nEqu2]();
	Ur_t = new double[nEqu2]();
	Ur_tt = new double[nEqu2]();

	getFixedBnd();

	nEnfMotn = nDISP + nVELO + nACCE;
	dispVeloAcce.setSize(nEnfMotn, 3);

	if (!onlyFixedBnd) {
		for (int i = 0; i < NNZ12; i++) {
			GD12[i] = alphad * GM12[i] + betad * GK12[i] + GfRatio * GK12[i] + GEfRatio * GD12[i];
		}
	}
}

void DTALoads::formRHS(const double tNow) {
	memset(RHS, 0, nEqu * sizeof(double));
	memset(Ur, 0, nEqu2 * sizeof(double));
	memset(Ur_t, 0, nEqu2 * sizeof(double));
	memset(Ur_tt, 0, nEqu2 * sizeof(double));
	iEnfMotn = -1;

	int NSID, DofID, CID, NumNode, NID, IEQ;
	double forceValue, dispValue, veloValue, acceValue;

	// Force Loads(*FORCE)
	for (int i = 0; i < nFORCE; i++) {
		NSID = FORCE[i].NSetsID;
		DofID = FORCE[i].DofID;
		forceValue = FORCE[i].Value1;
		CID = FORCE[i].CurveID1;
		NumNode = NSETS[NSID].nSetsNode;
		if (CID > 0) { CURVE[CID].GetValue(tNow, forceValue); }

		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			IEQ = NStat[TNode[NID]].DOF[DofID - 1];
			if (IEQ <= nEqu) { RHS[IEQ - 1] += forceValue; }
		}
	}

	if (!onlyFixedBnd) {
		// Disp Loads(*DISP), get Ur, Ur_t, Ur_tt based Ur
		for (int i = 0; i < nDISP; i++) {
			iEnfMotn++;
			NSID = DISP[i].NSetsID;
			DofID = DISP[i].DofID;
			dispValue = DISP[i].Value1;
			CID = DISP[i].CurveID1;
			NumNode = NSETS[NSID].nSetsNode;
			if (CID > 0) { CURVE[CID].GetValue(tNow, dispValue); }
			getDVAValue(1, B, dispValue, &dispVeloAcce(iEnfMotn, 0));

			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
				Ur[IEQ - 1] += dispVeloAcce(iEnfMotn, 0);
				Ur_t[IEQ - 1] += dispVeloAcce(iEnfMotn, 1);
				Ur_tt[IEQ - 1] += dispVeloAcce(iEnfMotn, 2);
			}
		}

		// Velo Loads(*VELO), get Ur, Ur_t, Ur_tt based Ur_t
		for (int i = 0; i < nVELO; i++) {
			iEnfMotn++;
			NSID = VELO[i].NSetsID;
			DofID = VELO[i].DofID;
			veloValue = VELO[i].Value1;
			CID = VELO[i].CurveID1;
			NumNode = NSETS[NSID].nSetsNode;
			if (CID > 0) { CURVE[CID].GetValue(tNow, veloValue); }
			getDVAValue(2, B, veloValue, &dispVeloAcce(iEnfMotn, 0));

			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
				Ur[IEQ - 1] += dispVeloAcce(iEnfMotn, 0);
				Ur_t[IEQ - 1] += dispVeloAcce(iEnfMotn, 1);
				Ur_tt[IEQ - 1] += dispVeloAcce(iEnfMotn, 2);
			}
		}

		// Acce Loads(*ACCE), get Ur, Ur_t, Ur_tt based Ur_tt
		for (int i = 0; i < nACCE; i++) {
			iEnfMotn++;
			NSID = ACCE[i].NSetsID;
			DofID = ACCE[i].DofID;
			acceValue = ACCE[i].Value1;
			CID = ACCE[i].CurveID1;
			NumNode = NSETS[NSID].nSetsNode;
			if (CID > 0) { CURVE[CID].GetValue(tNow, acceValue); }
			getDVAValue(3, B, acceValue, &dispVeloAcce(iEnfMotn, 0));

			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
				Ur[IEQ - 1] += dispVeloAcce(iEnfMotn, 0);
				Ur_t[IEQ - 1] += dispVeloAcce(iEnfMotn, 1);
				Ur_tt[IEQ - 1] += dispVeloAcce(iEnfMotn, 2);
			}
		}

		// calculate RHS
		// F = F - M12 * Ur_tt - C12 * Ur_t -K12 * Ur
		MS.dcsrmv("F", GM12, IROW12, JCOL12, nEqu, nEqu2, Ur_tt, RHS, -1.0, 1.0);
		MS.dcsrmv("F", GD12, IROW12, JCOL12, nEqu, nEqu2, Ur_t, RHS, -1.0, 1.0);
		MS.dcsrmv("F", GK12, IROW12, JCOL12, nEqu, nEqu2, Ur, RHS, -1.0, 1.0);
	}
}

void DTALoads::fini() {
	delete[] RHS;
	delete[] U;
	delete[] Ur;
	delete[] Ur_t;
	delete[] Ur_tt;

	dispVeloAcce.destroy();
}