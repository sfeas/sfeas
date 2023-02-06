//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is defenition of *LUMPM


#include "Keyword/KLUMPM.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KLUMPM::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->IsLumped;

	// check whether reasonable of reading information
	if (this->IsLumped < 0 || this->IsLumped > 1) {
		printMsg.error("IsLumped of *LUMPM is wrong", RowNum);
	}
}

KLUMPM* LUMPM = new KLUMPM;

void InitLUMPM(int& RowNum) {
	LUMPM->readKeyword(RowNum);
}