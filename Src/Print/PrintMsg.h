//Author		: Xiao Tuzi
//Date(Create)	: 12/22/2022
//Description	: this class is declaration of print message

#pragma once
#include <string>


class PrintMsg {
public:
	void logoInfo(const int label = 0);
	void modelInfo();
	void solveInfo();
	void timeInfo();

	void error(const std::string& str, const int& ID = -999999);
	void warning(const std::string& str, const int& ID = -999999);
};

extern PrintMsg printMsg;