//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Elem45

#pragma once
#include "Element/Element.h"

class Elem45 : public Element {
public:
	virtual void calElemMat(int iElem, bool isAssemble);	// calculate and assemble stiffness/mass/damp matrix
	virtual void calElemSTRA(int iElem);	// calculate element45 strain
	virtual void calElemSTRS(int iElem);	// calculate element45 stress
};
