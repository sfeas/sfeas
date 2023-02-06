//Author		: Xiao Tuzi
//Date(Create)	: 01/15/2023
//Description	: this program is definition of HistOut for FASolu


#pragma once
#include "Output/HistoryOutput/FAHistOut.h"
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

void FAHistOut::init() {
	Timer timerSolver("Output");

	if (nHISTOUT > 0){
		// open .plt result file
		hpltfile.open(InputFileName + ".hplt", std::ios::out);
		hpltfile.setf(std::ios::left);
		hpltfile.setf(std::ios::scientific);
		hpltfile.precision(8);

		// output title and header information
		hpltfile << "TITLE=\"History PostProcess\"" << "\n";
		hpltfile << "VARIABLES=\"Freq\",";
	}

	int NSID, NumNode, NID;
	for (int i = 0; i < nHISTOUT; i++){
		NSID = HISTOUT[i].NSetsID;
		NumNode = NSETS[NSID].nSetsNode;

		if (HISTOUT[i].OutType == "DISP") {
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				hpltfile << "\"" << NID << "-UX_Real\"," << "\"" << NID << "-UY_Real\"," << "\"" << NID << "-UZ_Real\",";
				hpltfile << "\"" << NID << "-UX_Imag\"," << "\"" << NID << "-UY_Imag\"," << "\"" << NID << "-UZ_Imag\",";
			}
		}
		else if (HISTOUT[i].OutType == "VELO") {
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				hpltfile << "\"" << NID << "-VX_Real\"," << "\"" << NID << "-VY_Real\"," << "\"" << NID << "-VZ_Real\",";
				hpltfile << "\"" << NID << "-VX_Imag\"," << "\"" << NID << "-VY_Imag\"," << "\"" << NID << "-VZ_Imag\",";
			}
		}
		else if (HISTOUT[i].OutType == "ACCE") {
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				hpltfile << "\"" << NID << "-AX_Real\"," << "\"" << NID << "-AY_Real\"," << "\"" << NID << "-AZ_Real\",";
				hpltfile << "\"" << NID << "-AX_Imag\"," << "\"" << NID << "-AY_Imag\"," << "\"" << NID << "-AZ_Imag\",";
			}
		}
		else if (HISTOUT[i].OutType == "STRA") {
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				hpltfile << "\"" << NID << "-EXX_Real\"," << "\"" << NID << "-EYY_Real\"," << "\"" << NID << "-EZZ_Real\","
					<< "\"" << NID << "-EXY_Real\"," << "\"" << NID << "-EYZ_Real\"," << "\"" << NID << "-EZX_Real\"," << "\"" << NID << "-EEQV_Real\",";
				hpltfile << "\"" << NID << "-EXX_Imag\"," << "\"" << NID << "-EYY_Imag\"," << "\"" << NID << "-EZZ_Imag\","
					<< "\"" << NID << "-EXY_Imag\"," << "\"" << NID << "-EYZ_Imag\"," << "\"" << NID << "-EZX_Imag\"," << "\"" << NID << "-EEQV_Imag\",";
			}
		}
		else if (HISTOUT[i].OutType == "STRE") {
			for (int j = 0; j < NumNode; j++) {
				NID = NSETS[NSID].SetsNodeID[j];
				hpltfile << "\"" << NID << "-SXX_Real\"," << "\"" << NID << "-SYY_Real\"," << "\"" << NID << "-SZZ_Real\","
					<< "\"" << NID << "-SXY_Real\"," << "\"" << NID << "-SYZ_Real\"," << "\"" << NID << "-SZX_Real\"," << "\"" << NID << "-SEQV\"";
				hpltfile << "\"" << NID << "-SXX_Imag\"," << "\"" << NID << "-SYY_Imag\"," << "\"" << NID << "-SZZ_Imag\","
					<< "\"" << NID << "-SXY_Imag\"," << "\"" << NID << "-SYZ_Imag\"," << "\"" << NID << "-SZX_Imag\"," << "\"" << NID << "-SEQV\"";
			}
		}
	}
	hpltfile << "\n";
	hpltfile << "ZONE I= " << nStep << ", F=POINT" << "\n";
}

void FAHistOut::exec(double fNow) {
	Timer timerSolver("Output");

	// output all variables
	if (isHistOut()) {
		hpltfile << std::setw(20) << fNow;

		for (int iLabel = 1; iLabel <= 2; iLabel++){
			// real part
			updateU(CDVA, fNow, iLabel);
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
		}
		hpltfile << "\n";
	}
}

void FAHistOut::fini() {
	Timer timerSolver("Output");

	// close .plt result file
	hpltfile.close();
}