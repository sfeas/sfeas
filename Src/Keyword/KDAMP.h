//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *DAMP


#pragma once
#include "Keyword/Keyword.h"

class KDAMP : public Keyword {
public:
	double alphad = 0.0;
	double betad = 0.0;
	double Gf = 0.0;
	double GEf = 0.0;
	double W3 = 0.0;
	double W4 = 0.0;
public:
	void readKeyword(int& RowNum);
};


extern KDAMP* DAMP;

void InitDAMP(int& RowNum);