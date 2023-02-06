//Author		: Xiao Tuzi
//Date(Create)	: 09/08/2022
//Description	: this program is calculating damping coefficient


#include "Solu/Tools/calDamp.h"
#include "Global/GlobalVar.h"
#include "Keyword/KANTYPE.h"
#include "Keyword/KDAMP.h"
#include "Keyword/KMDAMP.h"
#include "Keyword/KCURVE.h"
#include "Global/SolutionVar.h"

void calDamp(double &alphad, double& betad, double& Gf, double& GEf, double* E, int nMode, double* ksi) {
	int modalDampCurve = 0;
	double freq;

	// direct transient analysis
	if (ANTYPE->AnType == "DTA") {
		alphad = DAMP->alphad;
		betad = DAMP->betad;
		if (DAMP->W3 == 0.0) {
			Gf = 0.0;
		}
		else {
			Gf = DAMP->Gf / DAMP->W3;
		}
		if (DAMP->W4 == 0.0) {
			GEf = 0.0;
		}
		else {
			GEf = DAMP->Gf / DAMP->W4;
		}

	}
	// modal transient analysis
	else if (ANTYPE->AnType == "MTA") {
		alphad = DAMP->alphad;
		betad = DAMP->betad;
		if (DAMP->W3 == 0.0) {
			Gf = 0.0;
		}
		else {
			Gf = DAMP->Gf / DAMP->W3;
		}
		if (DAMP->W4 == 0.0) {
			GEf = 0.0;
		}
		else {
			GEf = DAMP->Gf / DAMP->W4;
		}
		modalDampCurve = MDAMP->CurveID;
	}

	// direct frequency analysis
	else if (ANTYPE->AnType == "DFA") {
		alphad = DAMP->alphad;
		betad = DAMP->betad;
		Gf = DAMP->Gf;
		GEf = DAMP->GEf;
	}

	// modal frequency analysis
	else if (ANTYPE->AnType == "MFA") {
		alphad = DAMP->alphad;
		betad = DAMP->betad;
		Gf = DAMP->Gf;
		GEf = DAMP->GEf;
		modalDampCurve = MDAMP->CurveID;
	}

	// handle modal damping coefficient
	if (modalDampCurve > 0) {
		for (int i = 0; i < nMode; i++) {
			freq = sqrt(E[i]) / 2.0 / PI;
			CURVE[modalDampCurve].GetValue(freq, ksi[i * nMode + i]);
		}
	}
	// constant modal damping
	else if (modalDampCurve == -1) {
		for (int i = 0; i < nMode; i++) {
			ksi[i * nMode + i] = MDAMP->consModalDamping;
		}
	}

	// no modal damping (modalDampCurve == 0)
	else{

	}

	return;
}


