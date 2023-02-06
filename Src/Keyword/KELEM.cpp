//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *ELEM


#include "Keyword/KELEM.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KELEM::readKeyword(int& RowNum) {
	RowNum += 1;
	datfile >> this->ElemID
		>> this->ETypeID
		>> this->MatID
		>> this->ENodeID[0]
		>> this->ENodeID[1]
		>> this->ENodeID[2]
		>> this->ENodeID[3]
		>> this->ENodeID[4]
		>> this->ENodeID[5]
		>> this->ENodeID[6]
		>> this->ENodeID[7];

	// check whether reasonable of reading information
	if (this->ElemID <= 0) {
		printMsg.error("NodeID of * ELEM is wrong", RowNum);
	}
	if (this->ETypeID <= 0) {
		printMsg.error("ETypeID of * ELEM is wrong", RowNum);
	}
	if (this->MatID <= 0) {
		printMsg.error("MatID of * ELEM is wrong", RowNum);
	}
	for (int i = 0; i < 8; i++) {
		if (this->ENodeID[i] <= 0) {
			printMsg.error("ENodeID of * ELEM is wrong", RowNum);
		}
	}
}

KELEM* ELEM;
CElemStat* EStat;
int* TElem;
int nElem = 0;			// total number of element

void InitELEM(int& RowNum) {
	ELEM = new KELEM[nElem + 1]();
	for (int i = 1; i <= nElem; i++) {
		ELEM[i].readKeyword(RowNum);
	}

	// initialize EStat
	EStat = new CElemStat[nElem + 1]();	// index begin 1

	// form TElem
	int MaxElemID = 0;
	for (int i = 1; i <= nElem; i++) {
		MaxElemID = std::max(MaxElemID, ELEM[i].ElemID);
	}

	TElem = new int[MaxElemID + 1]();  // index begin 1
	for (int i = 1; i <= nElem; i++) {
		if (TElem[ELEM[i].ElemID] != 0) {
			printMsg.error("ElemID is repeated in ELEM");
		}
		TElem[ELEM[i].ElemID] = i;
	}
}