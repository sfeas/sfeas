//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *NODESETS


#include "Keyword/KNSETS.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KNSETS::readKeyword(int& RowNum, int& nSetsNode) {

	const int nData = 8;	// a row can only store 8 datas
	int nRow = 0;			// the number of rows occupied by nodes in the nodesets
	int nNodeLastLine = 0;

	if (nSetsNode % nData == 0) {
		nRow = nSetsNode / nData;
	}
	else {
		nRow = nSetsNode / nData + 1;
	}

	this->nSetsNode = nSetsNode;
	this->SetsNodeID = new int[nSetsNode]();

	//only read 1 line data
	if (nRow == 1) {
		RowNum += 1;
		for (int j = 0; j < nSetsNode; j++) {
			datfile >> this->SetsNodeID[j];
		}
	}
	else {
		for (int i = 0; i < nRow - 1; i++) {
			RowNum += 1;
			// read the first (nRow-1) lines
			for (int j = 0; j < nData; j++) {
				datfile >> this->SetsNodeID[nData * i + j];
			}
		}

		// read last lines
		nNodeLastLine = nSetsNode - nData * (nRow - 1);
		RowNum += 1;
		for (int j = 0; j < nNodeLastLine; j++) {
			datfile >> this->SetsNodeID[nData * (nRow - 1) + j];
		}
	}

	// check whether reasonable of reading information
	for (int i = 0; i < nSetsNode; i++) {
		if (this->SetsNodeID[i] <= 0) {
			printMsg.error("SetsNodeID of *NODESETS is wrong", RowNum);
		}
	}
}

KNSETS* NSETS;
int nNSETS = 0;			// total number of node sets
std::map<std::string, int> NSetsMap;

void InitNSETS(int& RowNum) {
	NSETS = new KNSETS[nNSETS+1]();
	int nSetsNode = 0;
	std::string curveName;

	// index begin 1
	for (int i = 1; i <= nNSETS; i++) {
		RowNum += 1;
		datfile >> curveName >> nSetsNode;
		if (nSetsNode <= 0) {
			printMsg.error("nSetsNode of *NODESETS is wrong", RowNum);
		}
		NSetsMap[curveName] = i;
		NSETS[i].readKeyword(RowNum, nSetsNode);
	}
}