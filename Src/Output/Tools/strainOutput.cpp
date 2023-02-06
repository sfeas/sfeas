//Author		: Xiao Tuzi
//Date(Create)	: 01/16/2023
//Description	: this program is to output strain


#include "Output/Tools/strainOutput.h"
#include "Global/GlobalVar.h"

void strainOutput(int iNode, std::ofstream& outFile) {
	// output stress

	outFile << std::setw(20) << NStat[iNode].STRA[0]
		<< std::setw(20) << NStat[iNode].STRA[1]
		<< std::setw(20) << NStat[iNode].STRA[2]
		<< std::setw(20) << NStat[iNode].STRA[3]
		<< std::setw(20) << NStat[iNode].STRA[4]
		<< std::setw(20) << NStat[iNode].STRA[5]
		<< std::setw(20) << NStat[iNode].SEQV;

};