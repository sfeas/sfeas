//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Modal Frequency Analysis Solu

#pragma once
#include "Solu/Solu.h"

class MFASolu : public Solu {
public:
	void initSolu();	// Initialize Modal Frequency Analysis Solu
	void execSolu();	// Execute Modal Frequency Analysis Solu
	void finiSolu();	// Finish Modal Frequency Analysis Solu
};
