//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is declaration of Modal Transient Analysis Solu

#pragma once
#include "Solu/Solu.h"

class MTASolu : public Solu {
public:
	void initSolu();	// Initialize Modal Transient Analysis Solu
	void execSolu();	// Execute Modal Transient Analysis Solu
	void finiSolu();	// Finish Modal Transient Analysis Solu
};
