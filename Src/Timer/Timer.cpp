//Author		: Xiao Tuzi
//Date(Create)	: 12/22/2022
//Description	: this program is declaration of print Msg


#include "Timer/Timer.h"
#include "Timer/Tools/getWallTime.h"
#include "Timer/Tools/getCPUTime.h"

// timer point
double startWT;
double startCT;
double readWT;
double readCT;
double initWT;
double initCT;
double solveWT;
double solveCT;
double endWT;
double endCT;

// elapsed time
double formMatEWT = 0.0;
double formMatECT = 0.0;
double mSolverEWT = 0.0;
double mSolverECT = 0.0;
double outputEWT = 0.0;
double outputECT = 0.0;

Timer::Timer(std::string label) {
	this->startWT = getWallTime();
	this->startCT = getCPUTime();
	this->label = label;
};

Timer::~Timer() {
	double EWTime = getWallTime() - this->startWT;
	double ECTime = getCPUTime() - this->startCT;
	if (this->label == "EquaSolver") {
		mSolverEWT += EWTime;
		mSolverECT += ECTime;
	}
	else if (this->label == "Output") {
		outputEWT += EWTime;
		outputECT += ECTime;
	}
	else if (this->label == "AssGlobalMat") {
		formMatEWT += EWTime;
		formMatECT += ECTime;
	}
};

void Timer::recorder(double& WallTime, double& CPUTime) {
	WallTime = getWallTime();
	CPUTime = getCPUTime();
};

Timer timer;