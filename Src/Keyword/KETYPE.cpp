//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *ETYPE


#include "Keyword/KETYPE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KETYPE::readKeyword(int& RowNum) {
	std::string ElemTypeLabel;
	std::string NodeOfElemLabel;

	RowNum += 1;
	datfile >> this->ETypeID >> ElemTypeLabel >> NodeOfElemLabel;

	if (ElemTypeLabel == "SOLID45") {
		this->ElemType = 45;
		this->nNodeDof = 3;
	}
	else if (ElemTypeLabel == "SHELL63"){
		this->ElemType = 63;
		this->nNodeDof = 6;
	}
	else {
		printMsg.error("ElemType of *ETYPE is wrong", RowNum);
	}

	if (NodeOfElemLabel == "TETRA4") {
		this->nENode = 4;
		this->nIntePoint = 1;
	}
	//else if (NodeOfElemLabel == "PYRAM5") {
	//	this->nENode = 5;
	//}
	else if (NodeOfElemLabel == "PENTA6") {
		this->nENode = 6;
		this->nIntePoint = 4;
	}
	else if (NodeOfElemLabel == "HEXA8") {
		this->nENode = 8;
		this->nIntePoint = 8;
	}
	else {
		printMsg.error("nENode of *ETYPE is wrong", RowNum);
	}

	// check whether reasonable of reading information
	if (this->ETypeID <= 0) {
		printMsg.error("ETypeID of *ETYPE is wrong", RowNum);
	}
}

KETYPE* ETYPE;
int nETYPE = 0;			// total number of element Type

void InitETYPE(int& RowNum) {
	ETYPE = new KETYPE[nETYPE + 1];
	for (int i = 1; i <= nETYPE; i++) {
		ETYPE[i].readKeyword(RowNum);
	}
}