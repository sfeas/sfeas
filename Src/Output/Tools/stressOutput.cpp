//Author		: Xiao Tuzi
//Date(Create)	: 01/16/2023
//Description	: this program is to output stress


#include "Output/Tools/stressOutput.h"
#include "Global/GlobalVar.h"

void stressOutput(int iNode, std::ofstream& outFile) {
	// output stress

	outFile << std::setw(20) << NStat[iNode].STRE[0]
		<< std::setw(20) << NStat[iNode].STRE[1]
		<< std::setw(20) << NStat[iNode].STRE[2]
		<< std::setw(20) << NStat[iNode].STRE[3]
		<< std::setw(20) << NStat[iNode].STRE[4]
		<< std::setw(20) << NStat[iNode].STRE[5]
		<< std::setw(20) << NStat[iNode].SEQV;
};