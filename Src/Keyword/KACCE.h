//Author		: Xiao Tuzi
//Date(Create)	: 11/18/2022
//Description	: this program is declaration of *ACCE


#pragma once
#include "Keyword/Keyword.h"

class KACCE : public Keyword {
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


extern KACCE* ACCE;
extern int nACCE;

void InitACCE(int& RowNum);