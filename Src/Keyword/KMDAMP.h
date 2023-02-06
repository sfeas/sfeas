//Author		: Xiao Tuzi
//Date(Create)	: 12/13/2022
//Description	: this program is declaration of *MDAMP


#pragma once
#include "Keyword/Keyword.h"

class KMDAMP : public Keyword {
public:
	int CurveID = 0;
	double consModalDamping = 0.0;
public:
	void readKeyword(int& RowNum);
};


extern KMDAMP* MDAMP;

void InitMDAMP(int& RowNum);