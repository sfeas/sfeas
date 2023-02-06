//Author		: Xiao Tuzi
//Date(Create)	: 11/27/2022
//Description	: this program is abstract class of Loads


#pragma once

class Loads {
public:
	virtual void init() = 0;	// initial Loads
	virtual void formRHS(const double tNow = 0.0) = 0;	// formRHS
	virtual void fini() = 0;	// finish Loads

	virtual ~Loads() {}
};
