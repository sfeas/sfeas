//Author		: Xiao Tuzi
//Date(Create)	: 01/11/2023
//Description	: this program is declaration of LoadsList


#pragma once
#include "Loads/Loads.h"
#include "Loads/STALoads.h"
#include "Loads/MOALoads.h"
#include "Loads/DTALoads.h"
#include "Loads/MTALoads.h"
#include "Loads/DFALoads.h"
#include "Loads/MFALoads.h"

extern Loads* LoadsPtr;		// pointer of base class

extern STALoads STALoadsObj;		// object of derived class
extern MOALoads MOALoadsObj;
extern DTALoads DTALoadsObj;
extern MTALoads MTALoadsObj;
extern DFALoads DFALoadsObj;
extern MFALoads MFALoadsObj;

