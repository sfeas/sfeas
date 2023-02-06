//Author		: Xiao Tuzi
//Date(Create)	: 01/15/2023
//Description	: this program is declaration of General Output for MOASolu


#pragma once
#include "Output/GeneralOutput/GeneOut.h"

class MOAGeneOut : public GeneOut {
public:
	void init();	// initialize general output
	void exec(double tNow = 0.0);	// execute general output
	void fini();	// finish general output
};