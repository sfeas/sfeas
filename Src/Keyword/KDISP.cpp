//Author		: Xiao Tuzi
//Date(Create)	: 11/18/2022
//Description	: this program is defenition of *DISP


#include "Keyword/KDISP.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "Keyword/KNODE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KDISP::readKeyword(int& RowNum) {
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
		printMsg.error("NSetsID of *DISP is wrong", RowNum);
	}

	// judge DofLable
	if (DofLabel == "UX"){
		this->DofID = 1;
	}
	else if (DofLabel == "UY") {
		this->DofID = 2;
	}
	else if (DofLabel == "UZ") {
		this->DofID = 3;
	}
	else if (DofLabel == "RX") {
		this->DofID = 4;
	}
	else if (DofLabel == "RY") {
		this->DofID = 5;
	}
	else if (DofLabel == "RZ") {
		this->DofID = 6;
	}
	else {
		printMsg.error("DofID of *DISP is wrong", RowNum);
	}

	// judge Val1 and Val2 is int or curveName
	if (Val1.find("%") != Val1.npos) {
		Val1 = Val1.substr(1, Val1.length() - 2);
		if (CurveMap.find(Val1) != CurveMap.end()) {
			this->CurveID1 = CurveMap.find(Val1)->second;
		}
		else {
			printMsg.error("Value1 of *DISP is wrong", RowNum);
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
			printMsg.error("Value2 of *DISP is wrong", RowNum);
		}
	}
	else {
		this->Value2 = std::stod(Val2);
	}

}

KDISP* DISP;
int nDISP = 0;

void InitDISP(int& RowNum) {
	DISP = new KDISP[nDISP]();
	int NSID, DofID, NumNode, NID;

	for (int i = 0; i < nDISP; i++) {
		DISP[i].readKeyword(RowNum);
	}

	// handle DISP boundary
	for (int i = 0; i < nDISP; i++) {
		NSID = DISP[i].NSetsID;
		DofID = DISP[i].DofID;
		NumNode = NSETS[NSID].nSetsNode;
		for (int j = 0; j < NumNode; j++) {
			NID = NSETS[NSID].SetsNodeID[j];
			NStat[TNode[NID]].DOF[DofID - 1] = -1;				// <0 means DISP boundary
		}
	}
}