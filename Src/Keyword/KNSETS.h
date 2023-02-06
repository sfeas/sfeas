//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *NODESETS


#pragma once
#include "Keyword/Keyword.h"
#include <map>

class KNSETS : public Keyword {
public:
	int nSetsNode;
	int *SetsNodeID;
public:
	void readKeyword(int& RowNum, int& nSetsNode);
};

extern KNSETS* NSETS;
extern int nNSETS;			// total number of node sets
extern std::map<std::string, int> NSetsMap;

void InitNSETS(int& RowNum);