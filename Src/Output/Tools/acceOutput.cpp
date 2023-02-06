//Author		: Xiao Tuzi
//Date(Create)	: 01/16/2023
//Description	: this program is to output acceleration


#include "Output/Tools/acceOutput.h"
#include "Global/GlobalVar.h"

void acceOutput(int iNode, std::ofstream& outFile) {
	// output acceleration

	double magA = sqrt(pow(NStat[iNode].A[0], 2) + pow(NStat[iNode].A[1], 2) + pow(NStat[iNode].A[2], 2));
	outFile << std::setw(20) << NStat[iNode].A[0]
		<< std::setw(20) << NStat[iNode].A[1]
		<< std::setw(20) << NStat[iNode].A[2]
		<< std::setw(20) << magA;

};