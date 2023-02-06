//Author		: Xiao Tuzi
//Date(Create)	: 01/15/2023
//Description	: this program is definition of HistOut for DTASolu/MTASolu


#pragma once
#include "Output/HistoryOutput/TAHistOut.h"
#include "Timer/Timer.h"
#include "Global/Files.h"
#include "Global/GlobalVar.h"
#include "Keyword/KHISTOUT.h"
#include "Keyword/KNSETS.h"
#include "Global/SolutionVar.h"
#include "Output/Tools/isHistOut.h"
#include "Output/Tools/updateU.h"
#include "Output/Tools/calNSTRA.h"
#include "Output/Tools/calNSTRE.h"
#include "Output/Tools/dispOutput.h"
#include "Output/Tools/veloOutput.h"
#include "Output/Tools/acceOutput.h"
#include "Output/Tools/strainOutput.h"
#include "Output/Tools/stressOutput.h"

void TAHistOut::init() {
	Timer timerSolver("Output");

	if (nHISTOUT > 0){
		// open .plt result file
		hpltfile.open(InputFileName + ".hplt", std::ios::out);
		hpltfile.setf(std::ios::left);
		hpltfile.setf(std::ios::scientific);
		hpltfile.precision(8);

		// output title and header information
		hpltfile << "TITLE=\"History PostProcess\"" << "\n";
		hpltfile << "VARIABLES=\"Time\",";
	}

	int NSID, NumNode, NID;
	for (int i = 0; i < nHISTOUT; i++){
		NSID = HISTOUT[i].NSetsID;
		NumNode = NSETS[NSID].nSetsNode;

		if (HISTOUT[i].OutType == "DISP") {
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				hpltfile << "\"" << NID << "-UX\"," << "\"" << NID << "-UY\"," << "\"" << NID << "-UZ\"," << "\"" << NID << "-USUM\",";
			}
		}
		else if (HISTOUT[i].OutType == "VELO") {
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				hpltfile << "\"" << NID << "-VX\"," << "\"" << NID << "-VY\"," << "\"" << NID << "-VZ\"," << "\"" << NID << "-VSUM\",";
			}
		}
		else if (HISTOUT[i].OutType == "ACCE") {
			for (int i = 0; i < NumNode; i++) {
				NID = NSETS[NSID].SetsNodeID[i];
				hpltfile << "\"" << NID << "-AX\"," << "\"" << NID << "-AY\"," << "\"" << NID << "-AZ\"," << "\"" << NID << "-ASUM\",";
			}
		}
		else if (HISTOUT[i].OutType == "STRA") {
			for (int i = 0; i < NumNode; i++) {
				NID = NSETS[NSID].SetsNodeID[i];
				hpltfile << "\"" << NID << "-EXX\"," << "\"" << NID << "-EYY\"," << "\"" << NID << "-EZZ\","
					<< "\"" << NID << "-EXY\"," << "\"" << NID << "-EYZ\"," << "\"" << NID << "-EZX\"," << "\"" << NID << "-EEQV\",";
			}
		}
		else if (HISTOUT[i].OutType == "STRE") {
			for (int i = 0; i < NumNode; i++) {
				NID = NSETS[NSID].SetsNodeID[i];
				hpltfile << "\"" << NID << "-SXX\"," << "\"" << NID << "-SYY\"," << "\"" << NID << "-SZZ\","
					<< "\"" << NID << "-SXY\"," << "\"" << NID << "-SYZ\"," << "\"" << NID << "-SZX\"," << "\"" << NID << "-SEQV\"";
			}
		}
	}
	hpltfile << "\n";
	hpltfile << "ZONE I= " << nStep << ", F=POINT" << "\n";
}

void TAHistOut::exec(double tNow) {
	Timer timerSolver("Output");

	// output all variables
	if (isHistOut()) {
		hpltfile << std::setw(20) << tNow;

		updateU(DVA);
		for (int i = 0; i < nHISTOUT; i++) {
			if (HISTOUT[i].OutType == "STRA") { calNSTRA(); }
			if (HISTOUT[i].OutType == "STRE") { calNSTRE(); }
		}

		// for all nodes of NodeSets
		int NSID, NumNode, NID;
		for (int i = 0; i < nHISTOUT; i++) {
			NSID = HISTOUT[i].NSetsID;
			NumNode = NSETS[NSID].nSetsNode;

			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				NID = TNode[NID];
				if (HISTOUT[i].OutType == "DISP") {
					dispOutput(NID, hpltfile);
				}
				else if (HISTOUT[i].OutType == "VELO") {
					veloOutput(NID, hpltfile);
				}
				else if (HISTOUT[i].OutType == "ACCE") {
					acceOutput(NID, hpltfile);
				}
				else if (HISTOUT[i].OutType == "STRA") {
					strainOutput(NID, hpltfile);
				}
				else if (HISTOUT[i].OutType == "STRE") {
					stressOutput(NID, hpltfile);
				}
			}
		}
		hpltfile << "\n";
	}
}

void TAHistOut::fini() {
	Timer timerSolver("Output");

	// close .plt result file
	hpltfile.close();
}