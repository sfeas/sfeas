//Author		: Xiao Tuzi
//Date(Create)	: 08/28/2022
//Description	: this program is definition of calculating element strain


#include "Output/Tools/calESTRA.h"
#include "Global/GlobalVar.h"
#include "Initialize/ElemList.h"

void calESTRA() {
	int iET;
	
	for (int i = 1; i <= nElem; i++){
		iET = ELEM[i].ETypeID;
		ElemPtr[iET].P->calElemSTRA(i);
	}

	return;
}