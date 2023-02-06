//Author		: Xiao Tuzi
//Date(Create)	: 01/03/2023
//Description	: this program is declaration of Loads for STASolu


#pragma once
#include "Loads/Loads.h"

class STALoads : public Loads {
public:
	void init();
	void formRHS(const double tNow = 0.0);
	void fini();
};