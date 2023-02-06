//Author		: Xiao Tuzi
//Date(Create)	: 01/19/2023
//Description	: this program is to judge whether output general data in current step


#include "Output/Tools/isGeneOut.h"
#include "Keyword/KGENEOUT.h"
#include "Global/SolutionVar.h"

bool isGeneOut() {
	// judge whether output General data in current step
	bool IsGeneOut = false;
	if (nGENEOUT > 0) {
		if (GENEOUT[0].OutInterval == 0 || iStep == nStep) {
			if (iStep == nStep) { IsGeneOut = true; }		// only print last step
		}
		else if ((iStep - 1) % GENEOUT[0].OutInterval == 0) {
			IsGeneOut = true;
		}
	}
	return IsGeneOut;
}