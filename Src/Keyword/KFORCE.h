//Author		: Xiao Tuzi
//Date(Create)	: 11/19/2022
//Description	: this program is declaration of *FORCE


#pragma once
#include "Keyword/Keyword.h"

class KFORCE : public Keyword {
public:
	int NSetsID;
	int DofID;
	double Value1;
	double Value2;
	int CurveID1;
	int CurveID2;

public:
	void readKeyword(int& RowNum);
};


extern KFORCE* FORCE;
extern int nFORCE;

void InitFORCE(int& RowNum);