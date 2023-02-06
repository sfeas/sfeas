//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is declaration of Loads for MTASolu


#pragma once
#include "Loads/Loads.h"

class MTALoads : public Loads {
public:
	void init();
	void formRHS(const double tNow = 0.0);
	void fini();
};