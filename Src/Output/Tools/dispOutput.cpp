//Author		: Xiao Tuzi
//Date(Create)	: 01/16/2023
//Description	: this program is to output displacement


#include "Output/Tools/dispOutput.h"
#include "Global/GlobalVar.h"

void dispOutput(int iNode, std::ofstream& outFile) {
	// output displacement

	double magU = sqrt(pow(NStat[iNode].U[0], 2) + pow(NStat[iNode].U[1], 2) + pow(NStat[iNode].U[2], 2));
	outFile << std::setw(20) << NStat[iNode].U[0]
		<< std::setw(20) << NStat[iNode].U[1]
		<< std::setw(20) << NStat[iNode].U[2]
		<< std::setw(20) << magU;

};