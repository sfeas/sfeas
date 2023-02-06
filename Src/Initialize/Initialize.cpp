//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is definition of Initialize class


#include "Initialize/Initialize.h"
#include "Keyword/KANTYPE.h"
#include "Keyword/KETYPE.h"
#include "Keyword/KMAT.h"
#include "Initialize/SoluList.h"
#include "Initialize/ElemList.h"
#include "Initialize/MatList.h"
#include "Initialize/LoadsList.h"
#include "Initialize/OutputList.h"
#include "Initialize/Tools/guassIntePoint45.h"
#include "Initialize/initCSRIndex.h"

void Initialize::initSoluList() {
	// initialize Solu list
	if (ANTYPE->AnType == "STA") {
		SoluPtr = &STASoluObj;
	}
	else if (ANTYPE->AnType == "MOA") {
		SoluPtr = &MOASoluObj;
	}
	else if (ANTYPE->AnType == "DTA") {
		SoluPtr = &DTASoluObj;
	}
	else if (ANTYPE->AnType == "MTA") {
		SoluPtr = &MTASoluObj;
	}
	else if (ANTYPE->AnType == "DFA") {
		SoluPtr = &DFASoluObj;
	}
	else if (ANTYPE->AnType == "MFA") {
		SoluPtr = &MFASoluObj;
	}
}

void Initialize::initElemList() {
	// initialize element list
	// get guass integral point for different ETYPE

	int ET, nElemNode, nIntPt;
	ElemPtr = new ElemPointer[nETYPE + 1];
	for (int i = 1; i <= nETYPE; i++) {
		ET = ETYPE[i].ElemType;
		nElemNode = ETYPE[i].nENode;
		nIntPt = ETYPE[i].nIntePoint;
		dMatrix XP;
		double* WP;

		switch (ET) {
		// ETYPE is Elem45
		case 45:
			ElemPtr[i].XP3.setSize(3, nIntPt);
			ElemPtr[i].WP3 = new double[nIntPt]();
			XP.setSize(3, nIntPt);			// Guass integral point
			WP = new double[nIntPt]();		// Guass integral weight
			guassIntePoint45(nIntPt, XP, WP);

			ElemPtr[i].P = &Elem45Obj;
			for (int j = 0; j < nIntPt; j++) {
				ElemPtr[i].XP3(0, j) = XP(0, j);
				ElemPtr[i].XP3(1, j) = XP(1, j);
				ElemPtr[i].XP3(2, j) = XP(2, j);
				ElemPtr[i].WP3[j] = WP[j];
			}

			XP.destroy();
			delete[] WP;
			break;

		// ETYPE is Elem63
		case 63:
			ElemPtr[i].P = &Elem63Obj;
			break;

		default:
			break;
		}
	}
}

void Initialize::initMatList() {
	// initialize material list

	int MT;
	MatPtr = new MatPointer[nMAT + 1];
	for (int i = 1; i <= nMAT; i++) {
		MT = MAT[i].MatType;
		switch (MT) {
		case 1:
			MatPtr[i].P = &MatIsoObj;		// MAT is IsoElastic
			MatPtr[i].DMat.setSize(6, 6);
			MatPtr[i].P->calMatD(i, MatPtr[i].DMat);
			break;
		default:
			break;
		}
	}
}

void Initialize::initLoadsList() {
	// initialize loads list
	if (ANTYPE->AnType == "STA") {
		LoadsPtr = &STALoadsObj;
	}
	else if (ANTYPE->AnType == "MOA") {
		LoadsPtr = &MOALoadsObj;
	}
	else if (ANTYPE->AnType == "DTA") {
		LoadsPtr = &DTALoadsObj;
	}
	else if (ANTYPE->AnType == "MTA") {
		LoadsPtr = &MTALoadsObj;
	}
	else if (ANTYPE->AnType == "DFA") {
		LoadsPtr = &DFALoadsObj;
	}
	else if (ANTYPE->AnType == "MFA") {
		LoadsPtr = &MFALoadsObj;
	}
}

void Initialize::initOutputList() {
	// initialize output list
	if (ANTYPE->AnType == "STA") {
		GeneOutPtr = &STAGeneOutObj;
		HistOutPtr = &STAHistOutObj;
	}
	else if (ANTYPE->AnType == "MOA") {
		GeneOutPtr = &MOAGeneOutObj;
		HistOutPtr = &MOAHistOutObj;
	}
	else if (ANTYPE->AnType == "DTA") {
		GeneOutPtr = &TAGeneOutObj;
		HistOutPtr = &TAHistOutObj;
	}
	else if (ANTYPE->AnType == "MTA") {
		GeneOutPtr = &TAGeneOutObj;
		HistOutPtr = &TAHistOutObj;
	}
	else if (ANTYPE->AnType == "DFA") {
		GeneOutPtr = &FAGeneOutObj;
		HistOutPtr = &FAHistOutObj;
	}
	else if (ANTYPE->AnType == "MFA") {
		GeneOutPtr = &FAGeneOutObj;
		HistOutPtr = &FAHistOutObj;
	}
}

void Initialize::exec() {
	// initialize all above
	this->initElemList();
	this->initMatList();
	this->initLoadsList();
	this->initOutputList();

	initCSRIndex();
};			