//Author		: Xiao Tuzi
//Date(Create)	: 01/16/2023
//Description	: this program is to output velocity


#include "Output/Tools/veloOutput.h"
#include "Global/GlobalVar.h"

void veloOutput(int iNode, std::ofstream& outFile) {
	// output velocity

	double magV = sqrt(pow(NStat[iNode].V[0], 2) + pow(NStat[iNode].V[1], 2) + pow(NStat[iNode].V[2], 2));
	outFile << std::setw(20) << NStat[iNode].V[0]
		<< std::setw(20) << NStat[iNode].V[1]
		<< std::setw(20) << NStat[iNode].V[2]
		<< std::setw(20) << magV;
};