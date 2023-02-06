//Author		: Xiao Tuzi
//Date(Create)	: 08/27/2022
//Description	: this program is abstract base class of General Output


#pragma once

class GeneOut {
public:
	virtual void init() = 0;	// initialize general output
	virtual void exec(double tNow = 0.0) = 0;	// execute general output
	virtual void fini() = 0;	// finish general output

	virtual ~GeneOut() {}
};