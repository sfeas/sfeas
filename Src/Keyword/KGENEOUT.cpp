//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *GENEOUT


#include "Keyword/KGENEOUT.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KGENEOUT::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->OutType >> this->OutInterval;

	if (!(this->OutType == "DISP" || this->OutType == "VELO" || 
		this->OutType == "ACCE" || this->OutType == "STRA" || this->OutType == "STRE")){
		printMsg.error("OutType of *GENEOUT is wrong", RowNum);
	}

	// check whether reasonable of reading information
	if (this->OutInterval < 0) {
		printMsg.error("OutInterval of *GENEOUT is wrong", RowNum);
	}
}

KGENEOUT* GENEOUT;
int nGENEOUT = 0;

void InitGENEOUT(int& RowNum) {
	GENEOUT = new KGENEOUT[nGENEOUT]();
	for (int i = 0; i < nGENEOUT; i++){
		GENEOUT[i].readKeyword(RowNum);
	}
}