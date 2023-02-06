//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Modal Analysis Solu

#pragma once
#include "Solu/Solu.h"

class MOASolu : public Solu {
public:
	void initSolu();	// Initialize Modal Analysis Solu
	void execSolu();	// Execute Modal Analysis Solu
	void finiSolu();	// Finish Modal Analysis Solu
};
