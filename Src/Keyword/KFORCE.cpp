//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *FORCE


#include "Keyword/KFORCE.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KFORCE::readKeyword(int& RowNum) {
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
		printMsg.error("NSetsID of *FORCE is wrong", RowNum);
	}

	// judge DofLable
	if (DofLabel == "FX") {
		this->DofID = 1;
	}
	else if (DofLabel == "FY") {
		this->DofID = 2;
	}
	else if (DofLabel == "FZ") {
		this->DofID = 3;
	}
	else if (DofLabel == "MX") {
		this->DofID = 4;
	}
	else if (DofLabel == "MY") {
		this->DofID = 5;
	}
	else if (DofLabel == "MZ") {
		this->DofID = 6;
	}
	else {
		printMsg.error("DofID of *FORCE is wrong", RowNum);
	}

	// judge Val1 and Val2 is int or curveName
	if (Val1.find("%") != Val1.npos) {
		Val1 = Val1.substr(1, Val1.length() - 2);
		if (CurveMap.find(Val1) != CurveMap.end()){
			this->CurveID1 = CurveMap.find(Val1)->second;
		}
		else{
			printMsg.error("Value1 of *FORCE is wrong", RowNum);
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
			printMsg.error("Value2 of *FORCE is wrong", RowNum);
		}
	}
	else {
		this->Value2 = std::stod(Val2);
	}
}

KFORCE* FORCE;
int nFORCE = 0;

void InitFORCE(int& RowNum) {
	FORCE = new KFORCE[nFORCE]();

	for (int i = 0; i < nFORCE; i++) {
		FORCE[i].readKeyword(RowNum);
	}
}