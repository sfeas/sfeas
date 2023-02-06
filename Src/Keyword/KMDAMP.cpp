//Author		: Xiao Tuzi
//Date(Create)	: 12/13/2022
//Description	: this program is defenition of *MDAMP


#include "Keyword/KMDAMP.h"
#include "Keyword/KCURVE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KMDAMP::readKeyword(int& RowNum) {
	std::string Val;
	RowNum += 1;
	datfile >> Val;

	if (Val.find("%") != Val.npos) {
		Val = Val.substr(1, Val.length() - 2);
		if (CurveMap.find(Val) != CurveMap.end()) {
			this->CurveID = CurveMap.find(Val)->second;
		}
		else {
			printMsg.error("CurveID of *MDAMP is wrong", RowNum);
		}
	}
	else {
		// constant modal damping
		this->CurveID = -1;
		this->consModalDamping = std::stod(Val);
	}
}

KMDAMP* MDAMP = new KMDAMP;

void InitMDAMP(int& RowNum) {
	MDAMP->readKeyword(RowNum);
}