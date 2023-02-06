//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *EIGSLV


#include "Keyword/KEIGSLV.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KEIGSLV::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->SolverType;
	if (this->SolverType == "FST" || this->SolverType == "APK") {
	
	}
	else{
		printMsg.error("SolverType of *EIGSLV is wrong", RowNum);
	}
}

KEIGSLV* EIGSLV = new KEIGSLV;

void InitEIGSLV(int& RowNum) {
	EIGSLV->readKeyword(RowNum);
}