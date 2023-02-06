//Author		: Xiao Tuzi
//Date(Create)	: 08/28/2022
//Description	: this program is definition of calculating node strain


#include "Output/Tools/calNSTRA.h"
#include "Output/Tools/calESTRA.h"
#include "Global/GlobalVar.h"
#include "Keyword/KETYPE.h"

void calNSTRA() {
	// calculate element strain first(node)
	calESTRA();

	//	clear last results
	for (int i = 1; i <= nNode; i++) {
		for (int j = 0; j < 6; j++) {
			NStat[i].STRA[j] = 0.0;
		}
	}

	int ETID, nElemNode, NID;
	// distribute nodal strain(average)
	for (int iElem = 1; iElem <= nElem; iElem++) {
		ETID = ELEM[iElem].ETypeID;
		if (ETYPE[ETID].ElemType == 45) {
			nElemNode = 8;
		}
		else {
			nElemNode = ETYPE[ETID].nENode;
		}

		for (int j = 0; j < nElemNode; j++) {
			NID = ELEM[iElem].ENodeID[j];
			for (int i = 0; i < 6; i++) {
				NStat[TNode[NID]].STRA[i] += EStat[iElem].STRA[i][j + 1];
			}
		}

	}

	// calculate equivalent strain
	for (int iNode = 1; iNode <= nNode; iNode++) {
		for (int j = 0; j < 6; j++) {
			NStat[iNode].STRA[j] /= NStat[iNode].NREPT;
		}
		NStat[iNode].EEQV = pow((NStat[iNode].STRA[0] - NStat[iNode].STRA[1]), 2) +
			pow((NStat[iNode].STRA[1] - NStat[iNode].STRA[2]), 2) +
			pow((NStat[iNode].STRA[2] - NStat[iNode].STRA[0]), 2) +
			6.0 * pow(NStat[iNode].STRA[3], 2) +
			6.0 * pow(NStat[iNode].STRA[4], 2) +
			6.0 * pow(NStat[iNode].STRA[5], 2);
		NStat[iNode].EEQV = sqrt(0.5 * NStat[iNode].EEQV);
	}

	return;
}