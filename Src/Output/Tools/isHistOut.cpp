//Author		: Xiao Tuzi
//Date(Create)	: 01/19/2023
//Description	: this program is to judge whether output general data in current step


#include "Output/Tools/isHistOut.h"
#include "Keyword/KHISTOUT.h"
#include "Global/SolutionVar.h"

bool isHistOut() {
	// judge whether output history data in current step
	bool IsHistOut = false;
	if (nHISTOUT > 0) {
		if (iStep == nStep) {
			IsHistOut = true;
		}
		else if ((iStep - 1) % HISTOUT[0].OutInterval == 0) {
			IsHistOut = true;
		}
	}

	return IsHistOut;
}