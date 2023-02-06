//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is abstract class of Solu


#pragma once

class Solu {
public:
	virtual void initSolu() = 0;	// Initialize Solu
	virtual void execSolu() = 0;	// Execute Solu
	virtual void finiSolu() = 0;	// Finish Solu

	virtual ~Solu() {}
};
