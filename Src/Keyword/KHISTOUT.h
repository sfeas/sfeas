//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *HISTOUT


#pragma once
#include "Keyword/Keyword.h"

class KHISTOUT : public Keyword {
public:
	std::string OutType;
	int OutInterval;
	int NSetsID;
public:
	void readKeyword(int& RowNum);
};


extern KHISTOUT* HISTOUT;
extern int nHISTOUT;

void InitHISTOUT(int& RowNum);