//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *ELEM


#pragma once
#include "Keyword/Keyword.h"

class KELEM : public Keyword {
public:
	int ElemID;
	int ETypeID;
	int MatID;
	int ENodeID[8];
public:
	void readKeyword(int& RowNum);
};

class CElemStat {
public:
	double STRA[6][9] = { 0.0 };    // nodal strain(including average)
	double STRE[6][9] = { 0.0 };    // nodal stress(including average)
};


extern KELEM* ELEM;
extern CElemStat* EStat;
extern int* TElem;
extern int nElem;			// total number of element

void InitELEM(int& RowNum);