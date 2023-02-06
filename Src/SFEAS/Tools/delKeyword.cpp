//Author		: Xiao Tuzi
//Date(Create)	: 08/12/2022
//Description	: this program is to delete keyword pointer


#include "SFEAS/Tools/delKeyword.h"
#include "Keyword/KNODE.h"
#include "Keyword/KELEM.h"
#include "Keyword/KETYPE.h"
#include "Keyword/KMAT.h"
#include "Keyword/KNSETS.h"
#include "Keyword/KCURVE.h"
#include "Keyword/KDISP.h"
#include "Keyword/KVELO.h"
#include "Keyword/KACCE.h"
#include "Keyword/KFORCE.h"
#include "Keyword/KANTYPE.h"
#include "Keyword/KTIME.h"
#include "Keyword/KFREQ.h"
#include "Keyword/KEQUSLV.h"
#include "Keyword/KEIGSLV.h"
#include "Keyword/KLUMPM.h"
#include "Keyword/KMODEXT.h"
#include "Keyword/KTAINTM.h"
#include "Keyword/KTAINTP.h"
#include "Keyword/KDAMP.h"
#include "Keyword/KMDAMP.h"
#include "Keyword/KGENEOUT.h"
#include "Keyword/KHISTOUT.h"

void delKeyword() {
	// delete new keyword pointer

	delete[] NODE;
	delete[] NStat;
	delete[] TNode;

	delete[] ELEM;
	delete[] EStat;
	delete[] TElem;

	delete[] ETYPE;
	delete[] MAT;
	delete[] NSETS;
	delete[] CURVE;
	delete[] DISP;
	delete[] VELO;
	delete[] ACCE;
	delete[] FORCE;

	delete ANTYPE;
	delete TIME;
	delete FREQ;

	delete EQUSLV;
	delete EIGSLV;
	delete LUMPM;
	delete MODEXT;
	delete TAINTM;
	delete TAINTP;
	delete DAMP;
	delete MDAMP;

	delete[] GENEOUT;
	delete[] HISTOUT;
}