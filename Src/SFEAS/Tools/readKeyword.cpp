//Author		: Xiao Tuzi
//Date(Create)	: 08/12/2022
//Description	: this program is read Keyword of Input file


#pragma once
#include "SFEAS/Tools/readKeyword.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"
#include "Keyword/KNODE.h"
#include "Keyword/KELEM.h"
#include "Keyword/KETYPE.h"
#include "Keyword/KMAT.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "Keyword/KDISP.h"
#include "Keyword/KVELO.h"
#include "Keyword/KACCE.h"
#include "Keyword/KFORCE.h"
#include "Keyword/KANTYPE.h"
#include "Keyword/KTIME.h"
#include "Keyword/KFREQ.h"
#include "Keyword/KEQUSLV.h"
#include "Keyword/KEIGSLV.h"
#include "Keyword/KLUMPM.h"
#include "Keyword/KMODEXT.h"
#include "Keyword/KTAINTM.h"
#include "Keyword/KTAINTP.h"
#include "Keyword/KDAMP.h"
#include "Keyword/KMDAMP.h"
#include "Keyword/KGENEOUT.h"
#include "Keyword/KHISTOUT.h"

void readKeyword(int& RowNum, std::string& Keyword) {
	std::string Key = {};	// temp string

	if (Keyword == "*TITLE") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		RowNum = RowNum + 1;
		datfile >> Key;
	}
	else if (Keyword == "*NODE") {
		datfile >> nNode;	// get nNode
		std::cout << RowNum << "	" << Keyword << " " << nNode << "\n";
		logfile << RowNum << "	" << Keyword << " " << nNode << "\n";
		InitNODE(RowNum);
	}
	else if (Keyword == "*ELEM") {
		datfile >> nElem;  // get nElem
		std::cout << RowNum << "	" << Keyword << " " << nElem << "\n";
		logfile << RowNum << "	" << Keyword << " " << nElem << "\n";
		InitELEM(RowNum);
	}
	else if (Keyword == "*ETYPE") {
		datfile >> nETYPE;
		std::cout << RowNum << "	" << Keyword << " " << nETYPE << "\n";
		logfile << RowNum << "	" << Keyword << " " << nETYPE << "\n";
		InitETYPE(RowNum);
	}
	else if (Keyword == "*MAT") {
		datfile >> nMAT;
		std::cout << RowNum << "	" << Keyword << " " << nMAT << "\n";
		logfile << RowNum << "	" << Keyword << " " << nMAT << "\n";
		InitMAT(RowNum);
	}
	else if (Keyword == "*ANTYPE") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitANTYPE(RowNum);
	}
	else if (Keyword == "*EQUSLV") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitEQUSLV(RowNum);
	}
	else if (Keyword == "*EIGSLV") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitEIGSLV(RowNum);
	}
	else if (Keyword == "*NSETS") {
		datfile >> nNSETS;
		std::cout << RowNum << "	" << Keyword << " " << nNSETS << "\n";
		logfile << RowNum << "	" << Keyword << " " << nNSETS << "\n";
		InitNSETS(RowNum);
	}
	else if (Keyword == "*CURVE") {
		datfile >> nCURVE;
		std::cout << RowNum << "	" << Keyword << " " << nCURVE << "\n";
		logfile << RowNum << "	" << Keyword << " " << nCURVE << "\n";
		InitCURVE(RowNum);
	}
	else if (Keyword == "*DISP") {
		datfile >> nDISP;
		std::cout << RowNum << "	" << Keyword << " " << nDISP << "\n";
		logfile << RowNum << "	" << Keyword << " " << nDISP << "\n";
		InitDISP(RowNum);
	}
	else if (Keyword == "*VELO") {
		datfile >> nVELO;
		std::cout << RowNum << "	" << Keyword << " " << nVELO << "\n";
		logfile << RowNum << "	" << Keyword << " " << nVELO << "\n";
		InitVELO(RowNum);
	}
	else if (Keyword == "*ACCE") {
		datfile >> nACCE;
		std::cout << RowNum << "	" << Keyword << " " << nACCE << "\n";
		logfile << RowNum << "	" << Keyword << " " << nACCE << "\n";
		InitACCE(RowNum);
	}
	else if (Keyword == "*FORCE") {
		datfile >> nFORCE;
		std::cout << RowNum << "	" << Keyword << " " << nFORCE << "\n";
		logfile << RowNum << "	" << Keyword << " " << nFORCE << "\n";
		InitFORCE(RowNum);
	}
	else if (Keyword == "*LUMPM") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitLUMPM(RowNum);
	}
	else if (Keyword == "*MODEXT") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitMODEXT(RowNum);
	}
	else if (Keyword == "*TIME") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitTIME(RowNum);
	}
	else if (Keyword == "*FREQ") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitFREQ(RowNum);
	}
	else if (Keyword == "*TAINTM") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitTAINTM(RowNum);
	}
	else if (Keyword == "*TAINTP") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitTAINTP(RowNum);
	}
	else if (Keyword == "*DAMP") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitDAMP(RowNum);
	}
	else if (Keyword == "*MDAMP") {
		std::cout << RowNum << "	" << Keyword << "\n";
		logfile << RowNum << "	" << Keyword << "\n";
		InitMDAMP(RowNum);
	}
	else if (Keyword == "*GENEOUT") {
		datfile >> nGENEOUT;
		std::cout << RowNum << "	" << Keyword << " " << nGENEOUT << "\n";
		logfile << RowNum << "	" << Keyword << " " << nGENEOUT << "\n";
		InitGENEOUT(RowNum);
	}
	else if (Keyword == "*HISTOUT") {
		datfile >> nHISTOUT;
		std::cout << RowNum << "	" << Keyword << " " << nHISTOUT << "\n";
		logfile << RowNum << "	" << Keyword << " " << nHISTOUT << "\n";
		InitHISTOUT(RowNum);
	}
	else {
		printMsg.error("Unknown keyword : " + Keyword, RowNum);
	}
};