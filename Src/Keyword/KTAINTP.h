//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *TAINTP


#pragma once
#include "Keyword/Keyword.h"

class KTAINTP : public Keyword {
public:
	double alpha = 0.25250625;
	double delta = 0.505;
	double alphaF = 0.005;
	double alphaM = 0.0;
	double theta = 1.4;
public:
	void readKeyword(int& RowNum);
};


extern KTAINTP* TAINTP;

void InitTAINTP(int& RowNum);
