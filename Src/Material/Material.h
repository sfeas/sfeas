//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration bass class of Material


#pragma once
#include "DataType/Matrix.h"

class Material {
public:
	virtual void calMatD(int iMat, dMatrix& DMat) = 0;	// calculate Material D matrix
	virtual ~Material() {}
};