//Author		: Xiao Tuzi
//Date(Create)	: 08/12/2022
//Description	: this class is definition of SFEAS


#include "SFEAS/SFEAS.h"
#include "Global/GlobalVar.h"
#include "Initialize/Initialize.h"
#include "Initialize/SoluList.h"
#include "Global/Files.h"
#include "Timer/Timer.h"
#include "Print/PrintMsg.h"
#include "SFEAS/Tools/getFileFromWindows.h"
#include "SFEAS/Tools/readInput.h"
#include "SFEAS/Tools/delKeyword.h"

void SFEAS::init() {
	printMsg.logoInfo();	// print SFEAS information into screen
	std::cout << "Input file: ";
	std::string fileName = getFileFromWindow();	// choose file from window(with name extension)

	// get InputFileName from fileName, InputFileName without name extension
	size_t pos = fileName.find(".dat", 0);
	if (pos != fileName.npos) {
		InputFileName = fileName.substr(0, pos);
	}
	else {
		printMsg.error("Input file must be end with .dat, please check it");
	}

	// get CurrentDirectory from fileName, chaneg \\ to /
	CurrentDirectory = fileName.substr(0, fileName.rfind('\\'));
	
	size_t pos0 = 0;
	while ((pos0 = CurrentDirectory.find('\\', pos0)) != CurrentDirectory.npos) {
		CurrentDirectory.replace(pos0, 1, "/");
		pos0++;
	}

	timer.recorder(startWT, startCT);	// begin time recorder

	// open input file
	datfile.open(fileName, std::ios::in);
	if (!datfile.is_open()) {
		printMsg.error("Unable to open input file, please check it");
	}
	else {
		std::cout << fileName << "\n";

		// open .log and .err document
		logfile.open(InputFileName + ".log", std::ios::out);
		errfile.open(InputFileName + ".err", std::ios::out | std::ios::app);
		
		printMsg.logoInfo(1);	// print SFEAS information into log

		logfile << "Input file: " << fileName << "\n";

		// read all information from input file
		readInput();

		timer.recorder(readWT, readCT);	// read file time recorder

		printMsg.modelInfo();	// print model information

		// close .dat document
		datfile.close();
	}
}

void SFEAS::run() {
	// initialize Solu
	Initialize initialize;
	initialize.initSoluList();

	// perform analysis Solu
	SoluPtr->initSolu();
	timer.recorder(initWT, initCT);	// initial time recorder
	printMsg.solveInfo();	// print solve information

	std::cout << "\n********** Solution is running **********" << std::endl;
	logfile << "\n********** Solution is running **********" << std::endl;

	SoluPtr->execSolu();
	timer.recorder(solveWT, solveCT);	// solver time recorder

	SoluPtr->finiSolu();
	std::cout << "********** Solution is done **********" << std::endl;
	logfile << "********** Solution is done **********" << std::endl;
}

void SFEAS::fini() {
	// delete Keyword
	delKeyword();

	timer.recorder(endWT, endCT);	// end time recorder	

	printMsg.timeInfo();	// print time information

	// close .log and .err file
	if (logfile.is_open()) { logfile.close(); }
	if (errfile.is_open()) { errfile.close(); }
}