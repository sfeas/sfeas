//Author		: Xiao Tuzi
//Date(Create)	: 09/11/2022
//Description	: this program is declaration of class of History Output


#pragma once

class HistOut {
public:
	virtual void init() = 0;	// initialize history output
	virtual void exec(double fNow = 0.0) = 0;	// execute history output in FASolu
	virtual void fini() = 0;	// finish history output

	virtual ~HistOut() {}
};