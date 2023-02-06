//Author		: Xiao Tuzi
//Date(Create)	: 01/06/2023
//Description	: this program is definition of Loads for MOASolu


#pragma once
#include "Loads/MOALoads.h"
#include "Global/GlobalVar.h"
#include "MathSolver/MathSolver.h"
#include "Global/SolutionVar.h"

void MOALoads::init() {
	U = new double[nEqu]();
	Ur = new double[nEqu2]();
}

void MOALoads::formRHS(const double tNow) {
	// No Loads in MOASolu
}

void MOALoads::fini() {
	delete[] U;
	delete[] Ur;
}