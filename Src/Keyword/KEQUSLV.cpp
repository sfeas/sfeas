//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *EQUSLV


#include "Keyword/KEQUSLV.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KEQUSLV::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->SolverType;
	if (this->SolverType == "PDS" || this->SolverType == "MPS") {
		
	}
	else{
		printMsg.error("SolverType of *EQUSLV is wrong", RowNum);
	}
}

KEQUSLV* EQUSLV = new KEQUSLV;

void InitEQUSLV(int& RowNum) {
	EQUSLV->readKeyword(RowNum);
}