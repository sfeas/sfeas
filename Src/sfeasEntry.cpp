//Author		: Xiao Tuzi
//Date(Create)	: 08/12/2022
//Description	: this program is main program, entry point of SFEAS


#include "SFEAS/SFEAS.h"

int main() {
	SFEAS sfeas;
	
	sfeas.init();
	sfeas.run();
	sfeas.fini();
	
	return 0;
}
