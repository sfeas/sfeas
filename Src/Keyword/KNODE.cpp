//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *NODEINFO


#include "Keyword/KNODE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KNODE::readKeyword(int& RowNum) {

	RowNum += 1;
	datfile >> this->NodeID
		>> this->NodeCoord[0]
		>> this->NodeCoord[1]
		>> this->NodeCoord[2];

	// check whether reasonable of reading information
	if (this->NodeID <= 0) {
		printMsg.error("NodeID of *NODEINFO is wrong", RowNum);
	}
}

KNODE* NODE;
CNodeStat* NStat;
int* TNode;
int nNode = 0;			// total number of node

void InitNODE(int& RowNum) {
	NODE = new KNODE[nNode + 1]();
	for (int i = 1; i <= nNode; i++) {
		NODE[i].readKeyword(RowNum);
	}

	// initialize NStat
	NStat = new CNodeStat[nNode + 1]();	// index begin 1

	// form TNode
	int MaxNodeID = 0;
	for (int i = 1; i <= nNode; i++) {
		MaxNodeID = std::max(MaxNodeID, NODE[i].NodeID);
	}

	TNode = new int[MaxNodeID + 1]();  // index begin 1
	for (int i = 1; i <= nNode; i++) {
		if (TNode[NODE[i].NodeID] != 0) {
			printMsg.error("NodeID is repeated in NodeInfo");
		}
		TNode[NODE[i].NodeID] = i;
	}
}