//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of *GENEOUT


#pragma once
#include "Keyword/Keyword.h"

class KGENEOUT : public Keyword {
public:
	std::string OutType;
	int OutInterval;
public:
	void readKeyword(int& RowNum);
};


extern KGENEOUT* GENEOUT;
extern int nGENEOUT;

void InitGENEOUT(int& RowNum);
