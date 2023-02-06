//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *TAINTM


#include "Keyword/KTAINTM.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KTAINTM::readKeyword(int& RowNum) {
	std::string InteMethodLabel;

	RowNum += 1;
	datfile >> InteMethodLabel;

	if (InteMethodLabel == "NMK"){
		this->IntegralMethod = 1;
	}
	else if (InteMethodLabel == "WLS") {
		this->IntegralMethod = 2;
	}
	else if (InteMethodLabel == "HHT") {
		this->IntegralMethod = 3;
	}
	else {
		printMsg.error("IntegralMethod of *TAINTM is wrong", RowNum);
	}
}

KTAINTM* TAINTM = new KTAINTM;

void InitTAINTM(int& RowNum) {
	TAINTM->readKeyword(RowNum);
}