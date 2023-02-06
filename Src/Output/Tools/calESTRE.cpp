//Author		: Xiao Tuzi
//Date(Create)	: 08/28/2022
//Description	: this program is definition of calculating element stress


#include "Output/Tools/calESTRE.h"
#include "Global/GlobalVar.h"
#include "Initialize/ElemList.h"

void calESTRE() {
	int iET;

	for (int i = 1; i <= nElem; i++) {
		iET = ELEM[i].ETypeID;
		ElemPtr[iET].P->calElemSTRS(i);
	}

	return;
}