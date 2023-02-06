//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Elem63

#pragma once
#include "Element/Element.h"

class Elem63 : public Element {
public:
	virtual void calElemMat(int iElem, bool isAssemble);	// calculate and assemble stiffness/mass/damp matrix
	virtual void calElemSTRA(int iElem);	// calculate element63 strain
	virtual void calElemSTRS(int iElem);	// calculate element63 stress
};
