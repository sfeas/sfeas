//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is defenition of *MODEXT


#include "Keyword/KMODEXT.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KMODEXT::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->nModes
		>> this->MinFreq
		>> this->MaxFreq;

	// check whether reasonable of reading information
	if (this->nModes <= 0) {
		printMsg.error("nModes of *MODEXT is wrong", RowNum);
	}
	if (this->MinFreq < 0) {
		printMsg.error("MinFreq of *MODEXT is wrong", RowNum);
	}
	if (this->MaxFreq < 0) {
		printMsg.error("MaxFreq of *MODEXT is wrong", RowNum);
	}
	if (this->MaxFreq <= this->MinFreq) {
		printMsg.error("MaxFreq of *MODEXT is wrong", RowNum);
	}
}

KMODEXT* MODEXT = new KMODEXT;

void InitMODEXT(int& RowNum) {
	MODEXT->readKeyword(RowNum);

}