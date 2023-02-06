//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *EQUSLV


#pragma once
#include "Keyword/Keyword.h"

class KEQUSLV : public Keyword {
public:
	std::string SolverType = "MPS";
public:
	void readKeyword(int& RowNum);
};


extern KEQUSLV* EQUSLV;

void InitEQUSLV(int& RowNum);