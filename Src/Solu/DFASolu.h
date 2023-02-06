//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Direct Frequency Analysis Solu

#pragma once
#include "Solu/Solu.h"

class DFASolu : public Solu {
public:
	void initSolu();	// Initialize Direct Frequency Analysis Solu
	void execSolu();	// Execute Direct Frequency Analysis Solu
	void finiSolu();	// Finish Direct Frequency Analysis Solu
};
