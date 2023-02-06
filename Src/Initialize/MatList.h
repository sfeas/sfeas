//Author		: Xiao Tuzi
//Date(Create)	: 08/18/2022
//Description	: this program is declaration of MatList


#pragma once
#include "Material/Material.h"
#include "Material/MatIsoElastic.h"

class MatPointer {
public:
	Material* P;
	dMatrix DMat;
};

extern MatPointer* MatPtr;			// pointer of base class

extern MatIsoElastic MatIsoObj;		// object of derived class
