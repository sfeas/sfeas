//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is defenition of *CURVE


#include "Keyword/KCURVE.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void KCURVE::readKeyword(int& RowNum, int& nCurveData) {

	this->nCurveData = nCurveData;
	this->X = new double[nCurveData]();
	this->Y = new double[nCurveData]();

	//only read 1 line data
	for (int j = 0; j < nCurveData; j++) {
		RowNum += 1;
		datfile >> this->X[j] >> this->Y[j];
	}

	// check whether reasonable of reading information
	for (int i = 0; i < nCurveData; i++) {
		if (this->X[i] < 0) {
			printMsg.warning("X of *CURVE is wrong", RowNum);
		}
	}
}

void KCURVE::GetValue(const double& x, double& y) {
	int N = this->nCurveData;
	int j;
	double eps = 1.0e-14;

	// x is out of CoordinateX range
	if (x < (this->X[0] - eps) || x > (this->X[N - 1] + eps)) {
		y = 0.0;
		return;
	}
	// x in(X(j), X(j + 1)), 0 < j < N - 2
	else {
		for (j = 0; j <= N - 2; j++) {
			if (x >= this->X[j] && x <= this->X[j + 1]) { 
				break; 
			}
		}
	}

	y = this->Y[j] + (this->Y[j + 1] - this->Y[j])*(x - this->X[j]) / (this->X[j + 1] - this->X[j]);
}

KCURVE* CURVE;
int nCURVE = 0;			// total number of curve information
std::map<std::string, int> CurveMap;

void InitCURVE(int& RowNum) {
	CURVE = new KCURVE[nCURVE + 1]();
	int nCurveData = 0;
	std::string curveName;

	// index begin 1
	for (int i = 1; i <= nCURVE; i++) {
		RowNum += 1;
		datfile >> curveName >> nCurveData;
		if (nCurveData <= 0) {
			printMsg.warning("nCurveData of *CURVE is wrong", RowNum);
		}
		CurveMap[curveName] = i;
		CURVE[i].readKeyword(RowNum, nCurveData);
	}
}