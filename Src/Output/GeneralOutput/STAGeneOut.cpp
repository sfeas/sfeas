//Author		: Xiao Tuzi
//Date(Create)	: 01/15/2023
//Description	: this program is definition of GeneOut for STASolu


#pragma once
#include "Output/GeneralOutput/STAGeneOut.h"
#include "Timer/Timer.h"
#include "Global/Files.h"
#include "Global/GlobalVar.h"
#include "Keyword/KGENEOUT.h"
#include "Global/SolutionVar.h"
#include "Output/Tools/isGeneOut.h"
#include "Output/Tools/updateU.h"
#include "Output/Tools/calNSTRA.h"
#include "Output/Tools/calNSTRE.h"
#include "Output/Tools/dispOutput.h"
#include "Output/Tools/strainOutput.h"
#include "Output/Tools/stressOutput.h"

void STAGeneOut::init() {
	Timer timerSolver("Output");

	if (nGENEOUT > 0) {
		// open .plt result file
		pltfile.open(InputFileName + ".plt", std::ios::out);
		pltfile.setf(std::ios::left);
		pltfile.setf(std::ios::scientific);
		pltfile.precision(8);

		// output title and header information
		pltfile << "TITLE=\"General PostProcess\"" << "\n";
		pltfile << "VARIABLES=\"X\",\"Y\",\"Z\",";
	}

	for (int i = 0; i < nGENEOUT; i++){
		// output displacement
		if (GENEOUT[i].OutType == "DISP") {
			pltfile << "\"UX\",\"UY\",\"UZ\",\"USUM\"";
		}

		// output velocity
		else if (GENEOUT[i].OutType == "VELO") {
			printMsg.warning("Velocity OutType is not available in Static Analysis");
		}

		// output acceleration
		else if (GENEOUT[i].OutType == "ACCE") {
			printMsg.warning("Acceleration OutType is not available in Static Analysis");
		}

		// output strain
		else if (GENEOUT[i].OutType == "STRA") {
			pltfile << "\"EXX\",\"EYY\",\"EZZ\",\"EXY\",\"EYZ\",\"EZX\",\"EEQV\"";
		}

		// output stress
		else if (GENEOUT[i].OutType == "STRE") {
			pltfile << "\"SXX\",\"SYY\",\"SZZ\",\"SXY\",\"SYZ\",\"SZX\",\"SEQV\"";
		}
	}
	pltfile << "\n";
}

void STAGeneOut::exec(double tNow) {
	Timer timerSolver("Output");

	if (isGeneOut()) {
		pltfile << "ZONE T= \"" << iStep << "-" << tNow << "\",N=" << nNode << ",E=" << nElem << ",DATAPACKING=POINT,ZONETYPE=FEBRICK" << "\n";

		updateU(U);
		for (int i = 0; i < nGENEOUT; i++) {
			if (GENEOUT[i].OutType == "STRA") { calNSTRA(); }
			if (GENEOUT[i].OutType == "STRE") { calNSTRE(); }
		}

		// for all nodes
		for (int iNode = 1; iNode <= nNode; iNode++) {
			pltfile << std::setw(20) << NODE[iNode].NodeCoord[0] << std::setw(20) << NODE[iNode].NodeCoord[1] << std::setw(20) << NODE[iNode].NodeCoord[2];

			for (int i = 0; i < nGENEOUT; i++) {
				if (GENEOUT[i].OutType == "DISP") {
					dispOutput(iNode, pltfile);
				}
				else if (GENEOUT[i].OutType == "VELO") {
					// printMsg.warning("Velocity GeneOutType is not available in Static Analysis");
				}
				else if (GENEOUT[i].OutType == "ACCE") {
					// printMsg.warning("Acceleration GeneOutType is not available in Static Analysis");
				}
				else if (GENEOUT[i].OutType == "STRA") {
					strainOutput(iNode, pltfile);
				}
				else if (GENEOUT[i].OutType == "STRE") {
					stressOutput(iNode, pltfile);
				}
			}
			pltfile << "\n";
		}

		// output element information
		for (int i = 1; i <= nElem; i++) {
			for (int j = 0; j < 8; j++) {
				pltfile << ELEM[i].ENodeID[j] << "  ";
			}
			pltfile << "\n";
		}
	}
}

void STAGeneOut::fini() {
	Timer timerSolver("Output");

	// close .plt result file
	pltfile.close();
}