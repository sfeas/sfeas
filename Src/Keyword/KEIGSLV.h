//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *EIGSLV


#pragma once
#include "Keyword/Keyword.h"

class KEIGSLV : public Keyword {
public:
	std::string SolverType = "APK";
public:
	void readKeyword(int& RowNum);
};


extern KEIGSLV* EIGSLV;

void InitEIGSLV(int& RowNum);