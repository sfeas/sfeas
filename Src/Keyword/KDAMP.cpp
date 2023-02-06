//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *DAMP


#include "Keyword/KDAMP.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KDAMP::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->alphad >> this->betad >> this->Gf >> this->GEf >> this->W3 >> this->W4;

	// check whether reasonable of reading information
	if (this->alphad < 0) {
		printMsg.error("alphad of *DAMP is wrong", RowNum);
	}
	if (this->betad < 0) {
		printMsg.error("betad of *DAMP is wrong", RowNum);
	}
	if (this->Gf < 0) {
		printMsg.error("Gf of *DAMP is wrong", RowNum);
	}
	if (this->GEf < 0) {
		printMsg.error("GEf of *DAMP is wrong", RowNum);
	}
	if (this->W3 < 0) {
		printMsg.error("W3 of *DAMP is wrong", RowNum);
	}
	if (this->W4 < 0) {
		printMsg.error("W4 is *DAMP is wrong", RowNum);
	}
}

KDAMP* DAMP = new KDAMP;

void InitDAMP(int& RowNum) {
	DAMP->readKeyword(RowNum);
}