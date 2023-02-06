//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is defenition of *TIME


#include "Keyword/KTIME.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KTIME::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->StartTime
		>> this->EndTime
		>> this->TimeStep;

	// check whether reasonable of reading information
	if (this->StartTime < 0) {
		printMsg.warning("StartTime of *TIME is wrong", RowNum);
	}
	if (this->EndTime < 0) {
		printMsg.warning("EndTime of *TIME is wrong", RowNum);
	}
	if (this->TimeStep <= 0) {
		printMsg.warning("TimeStep of *TIME is wrong", RowNum);
	}
	if (this->StartTime >= this->EndTime) {
		printMsg.error("EndTime of *TIME is wrong", RowNum);
	}
}

KTIME* TIME;

void InitTIME(int& RowNum) {
	TIME = new KTIME;
	TIME->readKeyword(RowNum);

}