//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *MAT


#include "Keyword/KMAT.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KMAT::readKeyword(int& RowNum) {
	std::string MatTypeLabel;

	RowNum += 1;
	datfile >> this->MatID
		>> MatTypeLabel
		>> this->EX
		>> this->MU
		>> this->DENS
		>> this->GE;

	if (MatTypeLabel == "ISO"){
		this->MatType = 1;
	}
	else{
		printMsg.error("MAT of *MAT is wrong", RowNum);
	}

	// check whether reasonable of reading information
	if (this->MatID <= 0) {
		printMsg.error("MatID of *MAT is wrong", RowNum);
	}
	if (this->EX <= 0) {
		printMsg.warning("EX is of *MAT is wrong", RowNum);
	}
	if (this->MU <= 0) {
		printMsg.warning("MU is of *MAT is wrong", RowNum);
	}
	if (this->DENS <= 0) {
		printMsg.warning("DENS of *MAT is wrong", RowNum);
	}
	if (this->GE < 0) {
		printMsg.warning("GE of *MAT is wrong", RowNum);
	}
}

KMAT* MAT;
int nMAT = 0;			// total number of material Type

void InitMAT(int& RowNum) {
	MAT = new KMAT[nMAT + 1]();
	for (int i = 1; i <= nMAT; i++) {
		MAT[i].readKeyword(RowNum);
	}
}