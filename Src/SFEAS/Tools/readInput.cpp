//Author		: Xiao Tuzi
//Date(Create)	: 08/12/2022
//Description	: this program is read Input file


#include "SFEAS/Tools/readInput.h"
#include "SFEAS/Tools/readKeyword.h"
#include "Global/Files.h"
#include "Print/PrintMsg.h"

void readInput() {
	char K = {};
	std::string Keyword;
	int RowNum = 0;

	// read every line
	while (datfile >> Keyword) {
		RowNum = RowNum + 1;

		if (Keyword == "*END") {
			std::cout << RowNum << "	" << Keyword << "\n";
			std::cout << "Input file is read successful" << std::endl;
			logfile << "Input file is read successful" << std::endl;
			break;
		}

		K = Keyword[0];	// get first value of Keyword
		switch (K) {
		case '*':
			readKeyword(RowNum, Keyword);
			break;
		case '#':	// annotation
			getline(datfile, Keyword);
			break;
		default:
			printMsg.error("Unknown keyword: " + Keyword, RowNum);
			break;
		}
	}
}