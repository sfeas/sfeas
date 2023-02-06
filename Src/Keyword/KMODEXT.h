//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *MODEXT


#pragma once
#include "Keyword/Keyword.h"

class KMODEXT : public Keyword {
public:
	int nModes = 6;
	double MinFreq = 0.0;
	double MaxFreq = 1.0e6;
public:
	void readKeyword(int& RowNum);
};


extern KMODEXT* MODEXT;

void InitMODEXT(int& RowNum);