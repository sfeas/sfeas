//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *TAINTP


#include "Keyword/KTAINTP.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KTAINTP::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->alpha >> this->delta >> this->alphaF >> this->alphaM >> this->theta;

	// check whether reasonable of reading information
	if (this->alpha <= 0) {
		printMsg.error("alpha of *TAINTP is wrong", RowNum);
	}
	if (this->delta <= 0) {
		printMsg.error("beta of *TAINTP is wrong", RowNum);
	}
	if (this->alphaF < 0) {
		printMsg.error("alphaF of *TAINTP is wrong", RowNum);
	}
	if (this->alphaM < 0) {
		printMsg.error("alphaM of *TAINTP is wrong", RowNum);
	}
	if (this->theta <= 0) {
		printMsg.error("theta of *TAINTP is wrong", RowNum);
	}
}

KTAINTP* TAINTP = new KTAINTP;

void InitTAINTP(int& RowNum) {
	TAINTP->readKeyword(RowNum);
}