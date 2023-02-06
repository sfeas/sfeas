//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is definition of Loads for STASolu


#pragma once
#include "Loads/STALoads.h"
#include "Global/GlobalVar.h"
#include "Keyword/KFORCE.h"
#include "Keyword/KDISP.h"
#include "Keyword/KNSETS.h"
#include "MathSolver/MathSolver.h"
#include "Global/SolutionVar.h"
#include "Loads/Tools/getFixedBnd.h"

void STALoads::init() {
	RHS = new double[nEqu]();
	U = new double[nEqu]();
	Ur = new double[nEqu2]();

	getFixedBnd();
}

void STALoads::formRHS(const double tNow) {
	memset(RHS, 0, nEqu * sizeof(double));
	memset(Ur, 0, nEqu2 * sizeof(double));

	int NSID, DofID, NumNode, NID, IEQ;
	double forceValue, dispValue;

	// Force Loads(*FORCE)
	for (int i = 0; i < nFORCE; i++) {
		NSID = FORCE[i].NSetsID;
		DofID = FORCE[i].DofID;
		forceValue = FORCE[i].Value1;
		NumNode = NSETS[NSID].nSetsNode;
		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			IEQ = NStat[TNode[NID]].DOF[DofID - 1];
			if (IEQ <= nEqu) { RHS[IEQ - 1] += forceValue; }
		}
	}

	if (!onlyFixedBnd) {
		// Disp Loads(*DISP), get Ur
		for (int i = 0; i < nDISP; i++) {
			NSID = DISP[i].NSetsID;
			DofID = DISP[i].DofID;
			dispValue = DISP[i].Value1;
			NumNode = NSETS[NSID].nSetsNode;
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				IEQ = NStat[TNode[NID]].DOF[DofID - 1] - nEqu;
				Ur[IEQ - 1] = dispValue;
			}
		}

		// Velo Loads(*VELO)
		// No Velo Loads in STASolu

		// Acce Loads(*ACCE)
		// No Acce Loads in STASolu

		// calculate RHS
		// F = F - GK12 * Ur
		MS.dcsrmv("F", GK12, IROW12, JCOL12, nEqu, nEqu2, Ur, RHS, -1.0, 1.0);
	}
}

void STALoads::fini() {
	delete[] RHS;
	delete[] U;
	delete[] Ur;
}