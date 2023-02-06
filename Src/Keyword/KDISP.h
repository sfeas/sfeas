//Author		: Xiao Tuzi
//Date(Create)	: 11/18/2022
//Description	: this program is declaration of *DISP


#pragma once
#include "Keyword/Keyword.h"

class KDISP : public Keyword {
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


extern KDISP* DISP;
extern int nDISP;

void InitDISP(int& RowNum);