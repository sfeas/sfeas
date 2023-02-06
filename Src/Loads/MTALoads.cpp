//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is definition of Loads for MTASolu


#pragma once
#include "Loads/MTALoads.h"
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

void MTALoads::init() {
	RHS = new double[nEqu]();
	U = new double[nEqu]();
	Ur = new double[nEqu2]();
	Ur_t = new double[nEqu2]();
	Ur_tt = new double[nEqu2]();
	Us = new double[nEqu]();
	Us_t = new double[nEqu]();
	Us_tt = new double[nEqu]();

	getFixedBnd();

	nEnfMotn = nDISP + nVELO + nACCE;
	dispVeloAcce.setSize(nEnfMotn, 3);
	Ubb.setSize(nEnfMotn, nEqu);
	Ub.setSize(nEnfMotn, nEqu2);

	if (!onlyFixedBnd){
		for (int i = 0; i < NNZ; i++) {
			GD[i] = alphad * GM[i] + betad * GK[i] + GfRatio * GK[i] + GEfRatio * GD[i];
		}
		for (int i = 0; i < NNZ12; i++) {
			GD12[i] = alphad * GM12[i] + betad * GK12[i] + GfRatio * GK12[i] + GEfRatio * GD12[i];
		}
	}

	// get Ub from DISP, VELO, ACCE
	int NSID, DofID, NumNode, NID, IEQ;
	int count = -1;

	if (!onlyFixedBnd) {
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

		// get Ubb by solving GK * Ubb = -GK12 * Ub
		MS.decompose();
		for (int i = 0; i < nEnfMotn; i++) {
			MS.dcsrmv("F", GK12, IROW12, JCOL12, nEqu, nEqu2, &Ub(i, 0), RHS, -1.0, 0.0);
			MS.substitute(nEqu, RHS, &Ubb(i, 0));
		}
		MS.release();
	}
}

void MTALoads::formRHS(const double tNow) {
	memset(RHS, 0, nEqu * sizeof(double));
	memset(Us, 0, nEqu * sizeof(double));
	memset(Us_t, 0, nEqu * sizeof(double));
	memset(Us_tt, 0, nEqu * sizeof(double));
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
		// Disp Loads(*DISP), get Ur, Ur_t, Ur_tt and Us, Us_t, Us_tt
		for (int j = 0; j < nDISP; j++) {
			iEnfMotn++;
			CID = DISP[j].CurveID1;
			dispValue = DISP[j].Value1;
			if (CID > 0) { CURVE[CID].GetValue(tNow, dispValue); }
			getDVAValue(1, B, dispValue, &dispVeloAcce(iEnfMotn, 0));

			for (int i = 0; i < nEqu; i++) {
				Us[i] += dispVeloAcce(iEnfMotn, 0) * Ubb(iEnfMotn, i);
				Us_t[i] += dispVeloAcce(iEnfMotn, 1) * Ubb(iEnfMotn, i);
				Us_tt[i] += dispVeloAcce(iEnfMotn, 2) * Ubb(iEnfMotn, i);
			}
			for (int i = 0; i < nEqu2; i++) {
				Ur[i] += dispVeloAcce(iEnfMotn, 0) * Ub(iEnfMotn, i);
				Ur_t[i] += dispVeloAcce(iEnfMotn, 1) * Ub(iEnfMotn, i);
				Ur_tt[i] += dispVeloAcce(iEnfMotn, 2) * Ub(iEnfMotn, i);
			}
		}

		// Velo Loads(*VELO), get Ur, Ur_t, Ur_tt and Us, Us_t, Us_tt
		for (int j = 0; j < nVELO; j++) {
			iEnfMotn++;
			CID = VELO[j].CurveID1;
			veloValue = VELO[j].Value1;
			if (CID > 0) { CURVE[CID].GetValue(tNow, veloValue); }
			getDVAValue(2, B, veloValue, &dispVeloAcce(iEnfMotn, 0));

			for (int i = 0; i < nEqu; i++) {
				Us[i] += dispVeloAcce(iEnfMotn, 0) * Ubb(iEnfMotn, i);
				Us_t[i] += dispVeloAcce(iEnfMotn, 1) * Ubb(iEnfMotn, i);
				Us_tt[i] += dispVeloAcce(iEnfMotn, 2) * Ubb(iEnfMotn, i);
			}
			for (int i = 0; i < nEqu2; i++) {
				Ur[i] += dispVeloAcce(iEnfMotn, 0) * Ub(iEnfMotn, i);
				Ur_t[i] += dispVeloAcce(iEnfMotn, 1) * Ub(iEnfMotn, i);
				Ur_tt[i] += dispVeloAcce(iEnfMotn, 2) * Ub(iEnfMotn, i);
			}
		}

		// Acce Loads(*ACCE), get Ur, Ur_t, Ur_tt and Us, Us_t, Us_tt
		for (int j = 0; j < nACCE; j++) {
			iEnfMotn++;
			CID = ACCE[j].CurveID1;
			acceValue = ACCE[j].Value1;
			if (CID > 0) { CURVE[CID].GetValue(tNow, acceValue); }
			getDVAValue(3, B, acceValue, &dispVeloAcce(iEnfMotn, 0));

			for (int i = 0; i < nEqu; i++) {
				Us[i] += dispVeloAcce(iEnfMotn, 0) * Ubb(iEnfMotn, i);
				Us_t[i] += dispVeloAcce(iEnfMotn, 1) * Ubb(iEnfMotn, i);
				Us_tt[i] += dispVeloAcce(iEnfMotn, 2) * Ubb(iEnfMotn, i);
			}
			for (int i = 0; i < nEqu2; i++) {
				Ur[i] += dispVeloAcce(iEnfMotn, 0) * Ub(iEnfMotn, i);
				Ur_t[i] += dispVeloAcce(iEnfMotn, 1) * Ub(iEnfMotn, i);
				Ur_tt[i] += dispVeloAcce(iEnfMotn, 2) * Ub(iEnfMotn, i);
			}
		}

		// calculate RHS
		// F = F - M11*Us_tt - C11*Us_t - M12*Ur_tt - C12*Ur_t
		MS.dcsrmv("U", GM, IROW, JCOL, nEqu, nEqu, Us_tt, RHS, -1.0, 1.0);
		MS.dcsrmv("U", GD, IROW, JCOL, nEqu, nEqu, Us_t, RHS, -1.0, 1.0);
		MS.dcsrmv("F", GM12, IROW12, JCOL12, nEqu, nEqu2, Ur_tt, RHS, -1.0, 1.0);
		MS.dcsrmv("F", GD12, IROW12, JCOL12, nEqu, nEqu2, Ur_t, RHS, -1.0, 1.0);
	}
}

void MTALoads::fini() {
	delete[] RHS;
	delete[] U;
	delete[] Ur;
	delete[] Ur_t;
	delete[] Ur_tt;

	delete[] Us;
	delete[] Us_t;
	delete[] Us_tt;

	dispVeloAcce.destroy();
	Ubb.destroy();
	Ub.destroy();
}