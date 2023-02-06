//Author		: Xiao Tuzi
//Date(Create)	: 12/01/2022
//Description	: this program is defenition of of *VELO


#include "Keyword/KVELO.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "Keyword/KNODE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KVELO::readKeyword(int& RowNum) {
	std::string NSName;
	std::string Val1;
	std::string Val2;
	std::string DofLabel;

	RowNum += 1;
	datfile >> NSName >> DofLabel >> Val1 >> Val2;

	// judge NSetsID
	if (NSetsMap.find(NSName) != NSetsMap.end()) {
		this->NSetsID = NSetsMap.find(NSName)->second;
	}
	else {
		printMsg.error("NSetsID of *VELO is wrong", RowNum);
	}

	// judge DofLable
	if (DofLabel == "VX") {
		this->DofID = 1;
	}
	else if (DofLabel == "VY") {
		this->DofID = 2;
	}
	else if (DofLabel == "VZ") {
		this->DofID = 3;
	}
	else if (DofLabel == "RVX") {
		this->DofID = 4;
	}
	else if (DofLabel == "RVY") {
		this->DofID = 5;
	}
	else if (DofLabel == "RVZ") {
		this->DofID = 6;
	}
	else {
		printMsg.error("DofID of *VELO is wrong", RowNum);
	}

	// judge Val1 and Val2 is int or curveName
	if (Val1.find("%") != Val1.npos) {
		Val1 = Val1.substr(1, Val1.length() - 2);
		if (CurveMap.find(Val1) != CurveMap.end()) {
			this->CurveID1 = CurveMap.find(Val1)->second;
		}
		else {
			printMsg.error("Value1 of *VELO is wrong", RowNum);
		}
	}
	else {
		this->Value1 = std::stod(Val1);
	}

	if (Val2.find("%") != Val2.npos) {
		Val2 = Val2.substr(1, Val2.length() - 2);
		if (CurveMap.find(Val2) != CurveMap.end()) {
			this->CurveID2 = CurveMap.find(Val2)->second;
		}
		else {
			printMsg.error("Value2 of *VELO is wrong", RowNum);
		}
	}
	else {
		this->Value2 = std::stod(Val2);
	}
}

KVELO* VELO;
int nVELO = 0;

void InitVELO(int& RowNum) {
	VELO = new KVELO[nVELO]();
	int NSID, DofID, NumNode, NID;

	for (int i = 0; i < nVELO; i++) {
		VELO[i].readKeyword(RowNum);
	}

	// handle VELO boundary
	for (int i = 0; i < nVELO; i++) {
		NSID = VELO[i].NSetsID;
		DofID = VELO[i].DofID;
		NumNode = NSETS[NSID].nSetsNode;
		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			NStat[TNode[NID]].DOF[DofID - 1] = -1;				// <0 means VELO boundary
		}
	}
}