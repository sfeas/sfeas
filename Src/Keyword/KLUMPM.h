//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *LUMPM


#pragma once
#include "Keyword/Keyword.h"

class KLUMPM : public Keyword {
public:
	int IsLumped = 0;
public:
	void readKeyword(int& RowNum);
};


extern KLUMPM* LUMPM;

void InitLUMPM(int& RowNum);