//Author		: Xiao Tuzi
//Date(Create)	: 01/15/2023
//Description	: this program is declaration of History Output for DFASolu/MFASolu


#pragma once
#include "Output/HistoryOutput/HistOut.h"

class FAHistOut : public HistOut {
public:
	void init();	// initialize history output
	void exec(double fNow = 0.0);	// execute history output
	void fini();	// finish history output
};