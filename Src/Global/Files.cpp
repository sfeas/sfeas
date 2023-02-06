//Author		: Xiao Tuzi
//Date(Create)	: 08/15/2022
//Description	: this program is definetion of Files


#include "Global/Files.h"
#include <fstream>

/*-----------------------------input/output file-----------------------------*/
std::ifstream datfile;	// input file

std::ofstream logfile;
std::ofstream errfile;
std::ofstream fullfile;
std::ofstream pltfile;
std::ofstream hpltfile;