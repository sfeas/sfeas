//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *TIME


#pragma once
#include "Keyword/Keyword.h"

class KTIME : public Keyword {
public:
	double StartTime;
	double EndTime;
	double TimeStep;
public:
	void readKeyword(int& RowNum);
};


extern KTIME* TIME;

void InitTIME(int& RowNum);