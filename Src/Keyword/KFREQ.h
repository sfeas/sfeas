//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *FREQ


#pragma once
#include "Keyword/Keyword.h"

class KFREQ : public Keyword {
public:
	double StartFreq;
	double EndFreq;
	double FreqStep;
public:
	void readKeyword(int& RowNum);
};


extern KFREQ* FREQ;

void InitFREQ(int& RowNum);