//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *TAINTM


#pragma once
#include "Keyword/Keyword.h"

class KTAINTM : public Keyword {
public:
	int IntegralMethod = 1;
public:
	void readKeyword(int& RowNum);
};


extern KTAINTM* TAINTM;

void InitTAINTM(int& RowNum);