//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *HISTOUT


#include "Keyword/KHISTOUT.h"
#include "Keyword/KNSETS.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KHISTOUT::readKeyword(int& RowNum) {
	std::string NSName;

	RowNum += 1;
	datfile >> this->OutType >> this->OutInterval >> NSName;

	// judge NSetsID
	if (NSetsMap.find(NSName) != NSetsMap.end()) {
		this->NSetsID = NSetsMap.find(NSName)->second;
	}
	else {
		printMsg.error("NSetsID of *HISTOUT is wrong", RowNum);
	}

	// judge PlotType
	if (!(this->OutType == "DISP" || this->OutType == "VELO" ||
		this->OutType == "ACCE" || this->OutType == "STRA" || this->OutType == "STRE")) {
		printMsg.error("OutType of *HISTOUT is wrong", RowNum);
	}

	// check whether reasonable of reading information
	if (this->OutInterval <= 0) {
		printMsg.error("OutInterval of *HISTOUT is wrong", RowNum);
	}
}

KHISTOUT* HISTOUT;
int nHISTOUT = 0;

void InitHISTOUT(int& RowNum) {
	HISTOUT = new KHISTOUT[nHISTOUT]();
	for (int i = 0; i < nHISTOUT; i++){
		HISTOUT[i].readKeyword(RowNum);
	}
}