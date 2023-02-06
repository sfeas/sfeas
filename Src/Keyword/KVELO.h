//Author		: Xiao Tuzi
//Date(Create)	: 11/18/2022
//Description	: this program is declaration of *VELO


#pragma once
#include "Keyword/Keyword.h"

class KVELO : public Keyword {
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


extern KVELO* VELO;
extern int nVELO;

void InitVELO(int& RowNum);