//Author		: Xiao Tuzi
//Date(Create)	: 01/15/2023
//Description	: this program is declaration of OutputList


#pragma once
#include "Output/GeneralOutput/GeneOut.h"
#include "Output/HistoryOutput/HistOut.h"

#include "Output/GeneralOutput/STAGeneOut.h"
#include "Output/GeneralOutput/MOAGeneOut.h"
#include "Output/GeneralOutput/TAGeneOut.h"
#include "Output/GeneralOutput/FAGeneOut.h"

#include "Output/HistoryOutput/STAHistOut.h"
#include "Output/HistoryOutput/MOAHistOut.h"
#include "Output/HistoryOutput/TAHistOut.h"
#include "Output/HistoryOutput/FAHistOut.h"

extern GeneOut* GeneOutPtr;		// pointer of base class
extern HistOut* HistOutPtr;		// pointer of base class

extern STAGeneOut STAGeneOutObj;	// object of derived class
extern MOAGeneOut MOAGeneOutObj;
extern TAGeneOut TAGeneOutObj;
extern FAGeneOut FAGeneOutObj;

extern STAHistOut STAHistOutObj;
extern MOAHistOut MOAHistOutObj;
extern TAHistOut TAHistOutObj;
extern FAHistOut FAHistOutObj;