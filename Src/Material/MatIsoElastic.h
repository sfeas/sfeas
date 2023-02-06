//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of MatIsoElastic

#pragma once
#include "Material/Material.h"

class MatIsoElastic : public Material {
public:
	void calMatD(int iMat, dMatrix& DMat);	// calculate iostropic elastic material D matrix
};
