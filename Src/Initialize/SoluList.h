//Author		: Xiao Tuzi
//Date(Create)	: 08/18/2022
//Description	: this program is declaration of SoluList


#pragma once

#include "Solu/Solu.h"
#include "Solu/STASolu.h"
#include "Solu/MOASolu.h"
#include "Solu/DTASolu.h"
#include "Solu/MTASolu.h"
#include "Solu/DFASolu.h"
#include "Solu/MFASolu.h"

extern Solu* SoluPtr;		// pointer of base class

extern STASolu STASoluObj;	// object of derived class
extern MOASolu MOASoluObj;
extern DTASolu DTASoluObj;
extern MTASolu MTASoluObj;
extern DFASolu DFASoluObj;
extern MFASolu MFASoluObj;