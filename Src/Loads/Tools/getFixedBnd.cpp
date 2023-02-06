//Author		: Xiao Tuzi
//Date(Create)	: 12/26/2022
//Description	: this program is getting onlyFixedBnd in Boudary condition


#include "Loads/Tools/getFixedBnd.h"
#include "Global/SolutionVar.h"
#include "Keyword/KDISP.h"
#include "Keyword/KVELO.h"
#include "Keyword/KACCE.h"
#include "Global/Files.h"

void getFixedBnd() {
	onlyFixedBnd = true;

	// judge if only FixedBnd
	if (nVELO > 0 || nACCE > 0){
		onlyFixedBnd = false;
		return;
	}
	
	if (nDISP > 0){
		for (int i = 0; i < nDISP; i++) {
			if (DISP[i].CurveID1 <= 0 && DISP[i].Value1 == 0.0){
				onlyFixedBnd = true;
			}
			else
			{
				onlyFixedBnd = false;
				return;
			}
		}
	}

	if (onlyFixedBnd){
		std::cout << "  displacement loads condition is only fixed support" << "\n";
		logfile << "  displacement loads is is only fixed support" << "\n";
	}
}