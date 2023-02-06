//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *ANTYPE


#pragma once
#include "Keyword/Keyword.h"

class KANTYPE : public Keyword {
public:
	std::string AnType;
public:
	void readKeyword(int& RowNum);
};


extern KANTYPE* ANTYPE;

void InitANTYPE(int& RowNum);