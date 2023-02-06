//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of base class of Keyword

#pragma once
#include <string>

class Keyword {
public:
	void readKeyword(int& RowNum) {};	// read keyword
	virtual ~Keyword() {}
};