//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration abstract class of Element


#pragma once

class Element {
public:
	virtual void calElemMat(int iElem, bool isAssemble = true) = 0;		// calculate and assemble stiffness/mass/damp matrix
	virtual void calElemSTRA(int iElem) = 0;	// calculate element strain
	virtual void calElemSTRS(int iElem) = 0;	// calculate element stress

	virtual ~Element() {}
};