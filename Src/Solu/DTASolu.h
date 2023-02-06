//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Direct Transient Analysis Solu

#pragma once
#include "Solu/Solu.h"

class DTASolu : public Solu {
public:
	void initSolu();	// Initialize Direct Transient Analysis Solu
	void execSolu();	// Execute Direct Transient Analysis Solu
	void finiSolu();	// Finish Direct Transient Analysis Solu
};
