//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is declaration of Files

#pragma once

#include <fstream>
#include <iostream>

/*-----------------------------input/output file-----------------------------*/
extern std::ifstream datfile;	// input file

extern std::ofstream logfile;
extern std::ofstream errfile;
extern std::ofstream fullfile;
extern std::ofstream pltfile;
extern std::ofstream hpltfile;