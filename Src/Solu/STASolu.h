//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Static Analysis Solu

#pragma once
#include "Solu/Solu.h"

class STASolu : public Solu {
public:
	void initSolu();	// Initialize Static Analysis Solu
	void execSolu();	// Execute Static Analysis Solu
	void finiSolu();	// Finish Static Analysis Solu
};
