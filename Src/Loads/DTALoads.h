//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is declaration of Loads for DTASolu


#pragma once
#include "Loads/Loads.h"

class DTALoads : public Loads {
public:
	void init();
	void formRHS(const double tNow = 0.0);
	void fini();
};