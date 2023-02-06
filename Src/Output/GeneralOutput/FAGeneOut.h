//Author		: Xiao Tuzi
//Date(Create)	: 01/15/2023
//Description	: this program is declaration of General Output for DFASolu/MFASolu


#pragma once
#include "Output/GeneralOutput/GeneOut.h"

class FAGeneOut : public GeneOut {
public:
	void init();	// initialize general output
	void exec(double fNow = 0.0);	// execute general output
	void fini();	// finish general output
};