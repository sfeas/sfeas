//Author		: Xiao Tuzi
//Date(Create)	: 08/18/2022
//Description	: this program is declaration of Initialize class


#pragma once

class Initialize {
	// initialize: the pointer of base class is point to the object of derived class
public:
	void initSoluList();	// Init solution
	void initElemList();	// Init element list
	void initMatList();		// Init material list
	void initLoadsList();	// Init loads list
	void initOutputList();	// Init output list

	void exec();			// initialize all above
};