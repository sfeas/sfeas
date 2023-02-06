//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *CURVEINFO


#pragma once
#include "Keyword/Keyword.h"
#include <map>

class KCURVE : public Keyword {
public:
	int nCurveData;
	double* X;
	double* Y;
public:
	void readKeyword(int& RowNum, int& nSetsNode);
	void GetValue(const double& x, double& y);
};


extern KCURVE* CURVE;
extern int nCURVE;			// total number of curve information
extern std::map<std::string, int> CurveMap;

void InitCURVE(int& RowNum);