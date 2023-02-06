//Author		: Xiao Tuzi
//Date(Create)	: 10/27/2022
//Description	: this program is declaration of time recorder


#include <iostream>
#include <iomanip>
#include "Print/PrintMsg.h"
#include "Global/GlobalVar.h"
#include "Timer/Timer.h"
#include "Global/Files.h"
#include "Tools/getDataTime.h"
#include "Keyword/KMAT.h"
#include "Keyword/KETYPE.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "Keyword/KANTYPE.h"
#include "Keyword/KEQUSLV.h"
#include "Keyword/KEIGSLV.h"

void PrintMsg::logoInfo(const int label) {
	int year, month, day, hour, minute, second;
	getDataTime(year, month, day, hour, minute, second);

	if (label == 0) {
		std::cout << "===================================================================================" << "\n";
		std::cout << "========== Structural Finite Element Analysis System(SFEAS)  Release 1.0 ==========" << "\n";
		std::cout << "==========    Current Date" << "     ";
		std::cout << "TIME = "
			<< std::setfill('0') << std::setw(2) << hour << ":"
			<< std::setfill('0') << std::setw(2) << minute << ":"
			<< std::setfill('0') << std::setw(2) << second << "     ";
		std::cout << "DATE = "
			<< std::setfill('0') << std::setw(2) << month << "/"
			<< std::setfill('0') << std::setw(2) << day << "/"
			<< std::setfill('0') << std::setw(4) << year
			<< "     ==========\n";
		std::cout << "===================================================================================" << "\n";
		std::cout << "\n";
	}
	else if (label == 1) {
		logfile << "===================================================================================" << "\n";
		logfile << "========== Structural Finite Element Analysis System(SFEAS)  Release 1.0 ==========" << "\n";
		logfile << "==========    Current Date" << "     ";
		logfile << "TIME = "
			<< std::setfill('0') << std::setw(2) << hour << ":"
			<< std::setfill('0') << std::setw(2) << minute << ":"
			<< std::setfill('0') << std::setw(2) << second << "     ";
		logfile << "DATE = "
			<< std::setfill('0') << std::setw(2) << month << "/"
			<< std::setfill('0') << std::setw(2) << day << "/"
			<< std::setfill('0') << std::setw(4) << year
			<< "     ==========\n";
		logfile << "===================================================================================" << "\n";
		logfile << "\n";
	}
};

void PrintMsg::modelInfo() {
	std::cout << "\n========== Solution Model Statistics Information ==========" << "\n";
	std::cout << "total number of Node           = " << nNode << "\n";
	std::cout << "total number of Element        = " << nElem << "\n";
	std::cout << "total number of Element Type   = " << nETYPE << "\n";
	std::cout << "total number of Material Type  = " << nMAT << "\n";
	std::cout << "total number of NodeSets       = " << nNSETS << "\n";
	for (auto it : NSetsMap) {	// C++ 11
		std::cout << "  NodeSets name: " << it.first << ", including nodes = " << NSETS[it.second].nSetsNode << "\n";
	}
	std::cout << "total number of Curve          = " << nCURVE << "\n";
	for (auto it : CurveMap) {	// C++ 11
		std::cout << "  Curve name: " << it.first << ", including datas = " << CURVE[it.second].nCurveData << "\n";
	}

	logfile << "\n========== Solution Model Statistics Information ==========" << "\n";
	logfile << "total number of Node           = " << nNode << "\n";
	logfile << "total number of Element        = " << nElem << "\n";
	logfile << "total number of Element Type   = " << nETYPE << "\n";
	logfile << "total number of Material Type  = " << nMAT << "\n";
	logfile << "total number of NodeSets       = " << nNSETS << "\n";
	for (auto it : NSetsMap) {	// C++ 11
		logfile << "  NodeSets name: " << it.first << ", including nodes = " << NSETS[it.second].nSetsNode << "\n";
	}
	logfile << "total number of Curve          = " << nCURVE << "\n";
	for (auto it : CurveMap) {	// C++ 11
		logfile << "  Curve name: " << it.first << ", including datas = " << CURVE[it.second].nCurveData << "\n";
	}
};

void PrintMsg::solveInfo() {
	std::string analysisType;
	std::string solverType;
	bool IsEqu = false, IsEig = false;

	if (ANTYPE->AnType == "STA"){
		analysisType = "Static Analysis";
		IsEqu = true;
		
		if (EQUSLV->SolverType == "PDS") {
			solverType = "PARDISO in MKL";
		}
		else if (EQUSLV->SolverType == "MPS") {
			solverType = "MUMPS";
		}
	}
	else if (ANTYPE->AnType == "MOA"){
		analysisType = "Modal Analysis";
		IsEig = true;

		if (EIGSLV->SolverType == "FST") {
			solverType = "FEAST in MKL";
		}
		else if (EIGSLV->SolverType == "APK") {
			solverType = "ARPACK";
		}
	}
	else if (ANTYPE->AnType == "DTA") {
		analysisType = "Direct Transient Analysis";
		IsEqu = true;

		if (EQUSLV->SolverType == "PDS") {
			solverType = "PARDISO in MKL";
		}
		else if (EQUSLV->SolverType == "MPS") {
			solverType = "MUMPS";
		}
	}
	else if (ANTYPE->AnType == "MTA") {
		analysisType = "Modal Transient Analysis";
		IsEig = true;

		if (EIGSLV->SolverType == "FST") {
			solverType = "FEAST in MKL";
		}
		else if (EIGSLV->SolverType == "APK") {
			solverType = "ARPACK";
		}
	}
	else if (ANTYPE->AnType == "DFA") {
		analysisType = "Direct Frequency Analysis";
		IsEqu = true;

		if (EQUSLV->SolverType == "PDS") {
			solverType = "PARDISO in MKL";
		}
		else if (EQUSLV->SolverType == "MPS") {
			solverType = "MUMPS";
		}
	}
	else if (ANTYPE->AnType == "MFA") {
		analysisType = "Modal Frequency Analysis";
		IsEig = true;

		if (EIGSLV->SolverType == "FST") {
			solverType = "FEAST in MKL";
		}
		else if (EIGSLV->SolverType == "APK") {
			solverType = "ARPACK";
		}
	}
	double sparseRatio = NNZ * 100.0 / nEqu / nEqu;

	std::cout << "\n========== Solution Solver Statistics Information ==========" << "\n";
	std::cout << "analysis type                  = " << analysisType << "\n";
	if (IsEqu) {
		std::cout << "linear equation solver         = " << solverType << "\n";
	}
	if (IsEig) {
		std::cout << "eigen equation solver          = " << solverType << "\n";
	}
	std::cout << "total number of Equations      = " << nEqu << "\n";
	
	std::cout << "the non-zeros(%) of matrix     = " << NNZ << "(" << sparseRatio << "%)" << "\n";

	logfile << "\n========== Solution Solver Statistics Information ==========" << "\n";
	logfile << "analysis type                  = " << analysisType << "\n";
	if (IsEqu) {
		logfile << "linear equation solver         = " << solverType << "\n";
	}
	if (IsEig) {
		logfile << "eigen equation solver          = " << solverType << "\n";
	}
	logfile << "total number of Equations      = " << nEqu << "\n";
	logfile << "the non-zeros(%) of matrix     = " << NNZ << "(" << sparseRatio << "%)" << "\n";
};

void PrintMsg::timeInfo() {
	// read file elapsed time
	double readEWT =readWT - startWT;
	double readECT =readCT - startCT;
	// initial elapsed time
	double initEWT = initWT - readWT;
	double initECT = initCT - readCT;
	// solve elapsed time
	double solveEWT = solveWT - initWT;
	double solveECT = solveCT - initCT;
	// other elapsed time
	double otherEWT = endWT - solveWT;
	double otherECT = endCT - solveCT;

	// total elapsed time
	double totalEWT = endWT - startWT;
	double totalECT = endCT - startCT;
	// other solve elapsed time
	double otherSolveEWT = solveEWT - formMatEWT - mSolverEWT - outputEWT;
	double otherSolveECT = solveECT - formMatECT - mSolverECT - outputECT;

	std::cout << "\n========== Solution Time Statistics Information ==========" << "\n";
	std::cout << "read file Wall/CPU time(s)         = " << readEWT << "/" << readECT << "\n";
	std::cout << "initial Wall/CPU time(s)           = " << initEWT << "/" << initECT << "\n";
	std::cout << "solve Wall/CPU time(s)             = " << solveEWT << "/" << solveECT << "\n";
	std::cout << "    assemble global matrix Wall/CPU time(s)   = " << formMatEWT << "/" << formMatECT << "\n";
	std::cout << "    equation solver Wall/CPU time(s)          = " << mSolverEWT << "/" << mSolverECT << "\n";
	std::cout << "    output Wall/CPU time(s)                   = " << outputEWT << "/" << outputECT << "\n";
	std::cout << "    other solve Wall/CPU time(s)              = " << otherSolveEWT << "/" << otherSolveECT << "\n";
	std::cout << "other Wall/CPU time(s)             = " << otherEWT << "/" << otherECT << "\n";
	std::cout << "\ntotal solution Wall/CPU time(s)    = " << totalEWT << "/" << totalECT << "\n";

	logfile << "\n========== Solution Time Statistics Information ==========" << "\n";
	logfile << "read file Wall/CPU time(s)         = " << readEWT << "/" << readECT << "\n";
	logfile << "initial Wall/CPU time(s)           = " << initEWT << "/" << initECT << "\n";
	logfile << "solve Wall/CPU time(s)             = " << solveEWT << "/" << solveECT << "\n";
	logfile << "    assemble global matrix Wall/CPU time(s)   = " << formMatEWT << "/" << formMatECT << "\n";
	logfile << "    equation solver Wall/CPU time(s)          = " << mSolverEWT << "/" << mSolverECT << "\n";
	logfile << "    output Wall/CPU time(s)                   = " << outputEWT << "/" << outputECT << "\n";
	logfile << "    other solve Wall/CPU time(s)              = " << otherSolveEWT << "/" << otherSolveECT << "\n";
	logfile << "other Wall/CPU time(s)             = " << otherEWT << "/" << otherECT << "\n";
	logfile << "\ntotal solution Wall/CPU time(s)    = " << totalEWT << "/" << totalECT << "\n";
};

void PrintMsg::warning(const std::string& str, const int& ID) {
	int year, month, day, hour, minute, second;
	getDataTime(year, month, day, hour, minute, second);

	// print warning information to .err file
	errfile << "\n" << "*** WARNING ***";
	errfile << std::setfill(' ') << std::setw(30) << "TIME = "
		<< std::setfill('0') << std::setw(2) << hour << ":"
		<< std::setfill('0') << std::setw(2) << minute << ":"
		<< std::setfill('0') << std::setw(2) << second;
	errfile << std::setfill(' ') << std::setw(30) << "DATE = "
		<< std::setfill('0') << std::setw(2) << month << "/"
		<< std::setfill('0') << std::setw(2) << day << "/"
		<< std::setfill('0') << std::setw(4) << year << "\n";

	if (ID == -999999) {
		std::cout << "\n" << "*** WARNING ***" << "\n";
		std::cout << str << "\n";
		errfile << str << "\n";
	}
	else {
		std::cout << "\n" << "*** WARNING ***" << "\n";
		std::cout << str << "  " << ID << "\n";
		errfile << str << "  " << ID << "\n";
	}
};

void PrintMsg::error(const std::string& str, const int& ID) {
	int year, month, day, hour, minute, second;
	getDataTime(year, month, day, hour, minute, second);

	// print error information to .err file
	errfile << "\n" << "*** ERROR ***";
	errfile << std::setfill(' ') << std::setw(30) << "TIME = "
		<< std::setfill('0') << std::setw(2) << hour << ":"
		<< std::setfill('0') << std::setw(2) << minute << ":"
		<< std::setfill('0') << std::setw(2) << second;
	errfile << std::setfill(' ') << std::setw(30) << "DATE = "
		<< std::setfill('0') << std::setw(2) << month << "/"
		<< std::setfill('0') << std::setw(2) << day << "/"
		<< std::setfill('0') << std::setw(4) << year << "\n";

	if (ID == -999999) {
		std::cout << "\n" << "*** ERROR ***" << "\n";
		std::cout << str << "\n";
		errfile << str << "\n";
	}
	else {
		std::cout << "\n" << "*** ERROR ***" << "\n";
		std::cout << str << "  " << ID << "\n";
		errfile << str << "  " << ID << "\n";
	}

	exit(-1);
};

PrintMsg printMsg;