//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is defenition of *FREQ


#include "Keyword/KFREQ.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KFREQ::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->StartFreq
		>> this->EndFreq
		>> this->FreqStep;

	// check whether reasonable of reading information
	if (this->StartFreq < 0) {
		printMsg.warning("StartFreq of *FREQ is wrong", RowNum);
	}
	if (this->EndFreq < 0) {
		printMsg.warning("EndFreq of *FREQ is wrong", RowNum);
	}
	if (this->FreqStep <= 0) {
		printMsg.warning("FreqStep of *FREQ is wrong", RowNum);
	}
	if (this->StartFreq >= this->EndFreq) {
		printMsg.error("EndFreq of *FREQ is wrong", RowNum);
	}
}

KFREQ* FREQ;

void InitFREQ(int& RowNum) {
	FREQ = new KFREQ;
	FREQ->readKeyword(RowNum);

}