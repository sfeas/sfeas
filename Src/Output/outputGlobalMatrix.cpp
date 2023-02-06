//Author		: Xiao Tuzi
//Date(Create)	: 08/27/2022
//Description	: this program is definition of output global stiffness/mass/damp matrix


#include "Global/GlobalVar.h"
#include "Global/Files.h"
#include "Timer/Timer.h"

void outputGlobalMatrix() {
	
	bool IsOutput = false;	// whether output global matrix information
	if (IsOutput) {
		Timer timer("Output");

		std::cout << "output global matrix information..." << "\n";
		logfile << "output global matrix information..." << "\n";

		fullfile.open(InputFileName + ".full", std::ios::out);
		fullfile.setf(std::ios::left);
		fullfile.setf(std::ios::scientific);
		fullfile.precision(8);

		fullfile << "global matrix information, stored by upper triangle in CSR format" << "\n";
		fullfile << "NNZ is the non-zeros of global matrix, N is total number of equations" << "\n";
		fullfile << NNZ << nEqu << "\n";
		fullfile << "global stiff matrix[NNZ]    global mass matrix[NNZ]    global damp matrix[NNZ]    col index[NNZ]    row index[N+1]" << "\n";
		// GK[NNZ]    GM[NNZ]    GD[NNZ]    JCOL[NNZ]    IROW[nEqu+1]
	
		for (int i = 0; i < nEqu + 1; i++) {
			if (!(GK == nullptr)) {
				fullfile << std::setw(20) << GK[i];
			}
			else{
				fullfile << std::setw(20) << 0.0;
			}
			if (!(GM == nullptr)) {
				fullfile << std::setw(20) << GM[i];
			}
			else {
				fullfile << std::setw(20) << 0.0;
			}
			if (!(GD == nullptr)) { 
				fullfile << std::setw(20) << GD[i];
			}
			else {
				fullfile << std::setw(20) << 0.0;
			}
			fullfile << std::setw(20) << JCOL[i];
			fullfile << std::setw(20) << IROW[i]<< "\n";
		}
		for (int i = nEqu + 1; i < NNZ; i++) {
			if (!(GK == nullptr)) {
				fullfile << std::setw(20) << GK[i];
			}
			else {
				fullfile << std::setw(20) << 0.0;
			}
			if (!(GM == nullptr)) {
				fullfile << std::setw(20) << GM[i];
			}
			else {
				fullfile << std::setw(20) << 0.0;
			}
			if (!(GD == nullptr)) {
				fullfile << std::setw(20) << GD[i];
			}
			else {
				fullfile << std::setw(20) << 0.0;
			}
			fullfile << std::setw(20) << JCOL[i] << "\n";
		}
		fullfile.close();
	}
}


