//Author		: Xiao Tuzi
//Date(Create)	: 08/14/2022
//Description	: this program is declaration of *NODE


#pragma once
#include "Keyword/Keyword.h"

class KNODE : public Keyword {
public:
	int NodeID;
	double NodeCoord[3];
public:
	void readKeyword(int& RowNum);
};

class CNodeStat {
public:
    int DOF[6] = { 0 };         // node Dof ID
    int NDOFN = 0;              // num of sigle node dof
    int NREPT = 0;              // num of repeated node(a node is included in many elements)
    double U[6] = { 0.0 };      // nodal displacement
    double A[6] = { 0.0 };      // nodal velocity
    double V[6] = { 0.0 };      // nodal acceleration
    double STRA[6] = { 0.0 };   // nodal strain
    double EEQV = 0.0;          // nodal equivalent strain
    double STRE[6] = { 0.0 };   // nodal stress
    double SEQV = 0.0;          // nodal equivalent stress
};

extern KNODE* NODE;
extern CNodeStat* NStat;
extern int* TNode;
extern int nNode;			// total number of node

void InitNODE(int& RowNum);