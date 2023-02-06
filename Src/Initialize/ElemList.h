//Author		: Xiao Tuzi
//Date(Create)	: 08/18/2022
//Description	: this program is declaration of ElemList


#pragma once
#include "Element/Element.h"
#include "Element/Elem45.h"
#include "Element/Elem63.h"
#include "DataType/Matrix.h"

class ElemPointer {
public:
	Element* P;
	dMatrix XP3;		// Guass integral point for 3D elemnet
	double *WP3;		// Guass integral weight for 3D elemnet
};

extern ElemPointer* ElemPtr;	// pointer of base class

extern Elem45 Elem45Obj;		// object of derived class
extern Elem63 Elem63Obj;		// object of derived class