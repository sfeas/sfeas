//Author		: Xiao Tuzi
//Date(Create)	: 10/27/2022
//Description	: this program is declaration of time recorder


#pragma once
#include <string>

// timer point
extern double startWT;
extern double startCT;
extern double readWT;
extern double readCT;
extern double initWT;
extern double initCT;
extern double solveWT;
extern double solveCT;
extern double endWT;
extern double endCT;

// elapsed time
extern double formMatEWT;
extern double formMatECT;
extern double mSolverEWT;
extern double mSolverECT;
extern double outputEWT;
extern double outputECT;

class Timer {
private:
	double startWT;
	double startCT;
	std::string label;

public:
	Timer(std::string label = "");
	~Timer();

	void recorder(double& WallTime, double& CPUTime);
};

extern Timer timer;