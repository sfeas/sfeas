//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *ETYPE


#pragma once
#include "Keyword/Keyword.h"

class KETYPE : public Keyword {
public:
	int ETypeID;
	int ElemType;
	int nENode;

	int nIntePoint;		// no need input
	int nNodeDof;
public:
	void readKeyword(int& RowNum);
};


extern KETYPE* ETYPE;
extern int nETYPE;			// total number of element Type

void InitETYPE(int& RowNum);