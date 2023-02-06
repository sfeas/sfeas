//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *MAT


#pragma once
#include "Keyword/Keyword.h"

class KMAT : public Keyword {
public:
	int MatID;
	int MatType;
	double EX;
	double MU;
	double DENS;
	double GE;
public:
	void readKeyword(int& RowNum);
};


extern KMAT* MAT;
extern int nMAT;			// total number of material Type

void InitMAT(int& RowNum);