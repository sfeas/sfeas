//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *ANTYPE


#include "Keyword/KANTYPE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KANTYPE::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->AnType;
	if (this->AnType == "STA" || this->AnType == "MOA" || this->AnType == "DTA"
		|| this->AnType == "MTA" || this->AnType == "DFA" || this->AnType == "MFA") {

	}
	else {
		printMsg.error("AnTypeID of *ANTYPE is wrong", RowNum);
	}
}

KANTYPE* ANTYPE;

void InitANTYPE(int& RowNum) {
	ANTYPE = new KANTYPE;
	ANTYPE->readKeyword(RowNum);
}