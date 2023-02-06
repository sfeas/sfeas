//Author		: Xiao Tuzi
//Date(Create)	: 01/11/2023
//Description	: this program is definition of initialize CSR Index


#pragma once
#include "Initialize/initCSRIndex.h"
#include "Global/GlobalVar.h"
#include "Keyword/KETYPE.h"

struct ConnectedNode {
	std::set<int> NodeList;		// 记录每个节点关联的节点列表
};

void initCSRIndex() {
	// initialize CSR format, get nEqu, IROW, JCOL and NNZ
	int IND, IEQ, ETID, nNodeDof, nElemNode;

	//------------------------------------------------------------------------ -
	// initialize nEqu, NodeList
	std::set<int> NODES;
	struct ConnectedNode* conNode = new ConnectedNode[nNode]();

	// get NodeList
	for (int iElem = 1; iElem <= nElem; iElem++) {
		ETID = ELEM[iElem].ETypeID;
		if (ETYPE[ETID].ElemType == 45) { nElemNode = 8; }
		nNodeDof = ETYPE[ETID].nNodeDof;         // num of Node Dof

		for (int i = 0; i < nElemNode; i++) {
			IND = ELEM[iElem].ENodeID[i];
			NStat[TNode[IND]].NDOFN = nNodeDof;
			for (int ii = 0; ii < nElemNode; ii++) {
				conNode[TNode[IND] - 1].NodeList.insert(ELEM[iElem].ENodeID[ii]);
			}
			auto pairNODES = NODES.insert(IND);	// pairNODES.second表示插入的状态, 若插入失败(已经存在该值)则为false
			// 同一个单元内, 重复节点不算, 获取每个节点重复次数,以方便计算节点应力/应变
			if (pairNODES.second) { NStat[TNode[IND]].NREPT += 1; }
		}
		NODES.clear();
	}

	// get nEqu
	nEqu = 0;
	for (int i = 1; i <= nNode; i++) {
		for (int j = 0; j < NStat[i].NDOFN; j++) {
			if (NStat[i].DOF[j] == 0) {			// == 0, means it is freedom Dof
				nEqu = nEqu + 1;
				NStat[i].DOF[j] = nEqu;
			}
		}
	}

	// get nDof
	nDof = nEqu;
	for (int i = 1; i <= nNode; i++) {
		for (int j = 0; j < NStat[i].NDOFN; j++) {
			if (NStat[i].DOF[j] < 0) {			// < 0, means it is DISP/VELO/ACCE constraint
				nDof = nDof + 1;
				NStat[i].DOF[j] = nDof;
			}
		}
	}

	nEqu2 = nDof - nEqu;

	//------------------------------------------------------------------------ -
	// 基于NodeList形成自由度关联列表DofList(去除约束), 
	// 然后记录每个方程关联的方程个数EquNum, 即总体矩阵刚度中每行非零元素(考虑对称性只存储上三角)
	std::vector<int> EquNum_11(nEqu, 0);	// 每个方程关联的方程个数
	std::vector<int> EquNum_12(nEqu, 0);
	std::vector<int> EquNum_22(nDof - nEqu, 0);
	std::vector<int> JCOL_11, JCOL_12, JCOL_22;
	std::set<int> DofList;				// 记录每个节点关联的节点自由度列表
	int nRowDof_11, nRowDof_12, nRowDof_22;

	for (int i = 1; i <= nNode; i++) {
		// get DofList
		DofList.clear();
		for (auto it = conNode[i - 1].NodeList.begin(); it != conNode[i - 1].NodeList.end(); it++) {
			IEQ = TNode[*it];
			for (int j = 0; j < NStat[IEQ].NDOFN; j++) {
				DofList.insert(NStat[IEQ].DOF[j]);
			}
		}

		// get JCOL_11, JCOL_12, JCOL_22 and EquNum_11, EquNum_12, EquNum_22 
		for (int j = 0; j < NStat[i].NDOFN; j++) {
			IEQ = NStat[i].DOF[j];
			// freedom Dof, form 11 and 12
			if (IEQ <= nEqu) {
				nRowDof_11 = 0;
				nRowDof_12 = 0;
				// storage upper triangle
				for (auto k = DofList.lower_bound(IEQ); k != DofList.end(); k++) {
					if (*k <= nEqu) {
						nRowDof_11++;
						JCOL_11.push_back(*k);
					}
					else {
						nRowDof_12++;
						JCOL_12.push_back(*k - nEqu);
					}
				}
				EquNum_11[IEQ - 1] = nRowDof_11;
				EquNum_12[IEQ - 1] = nRowDof_12;
			}
			// restrain Dof, form 22
			else {
				nRowDof_22 = 0;
				// storage upper triangle
				for (auto k = DofList.lower_bound(IEQ); k != DofList.end(); k++) {
					nRowDof_22++;
					JCOL_22.push_back(*k - nEqu);
				}
				EquNum_22[IEQ - nEqu - 1] = nRowDof_22;
			}
		}
	}

	//------------------------------------------------------------------------ -
	// initialize IROW
	IROW = new int[nEqu + 1];
	IROW[0] = 1;
	for (int i = 0; i < nEqu; i++) {
		IROW[i + 1] = IROW[i] + EquNum_11[i];
	}

	IROW12 = new int[nEqu + 1];
	IROW12[0] = 1;
	for (int i = 0; i < nEqu; i++) {
		IROW12[i + 1] = IROW12[i] + EquNum_12[i];
	}

	IROW22 = new int[nEqu2 + 1];
	IROW22[0] = 1;
	for (int i = 0; i < nEqu2; i++) {
		IROW22[i + 1] = IROW22[i] + EquNum_22[i];
	}

	//------------------------------------------------------------------------ -
	// initialize JCOL
	NNZ = IROW[nEqu] - 1;	// NNZ = JCOL0.size()
	NNZ12 = IROW12[nEqu] - 1;
	NNZ22 = IROW22[nEqu2] - 1;

	JCOL = new int[NNZ];
	JCOL12 = new int[NNZ12];
	JCOL22 = new int[NNZ22];
	// get JCOL
	if (!JCOL_11.empty()) {
		memcpy(JCOL, &JCOL_11[0], NNZ * sizeof(int));
	}
	if (!JCOL_12.empty()) {
		memcpy(JCOL12, &JCOL_12[0], NNZ12 * sizeof(int));
	}
	if (!JCOL_22.empty()) {
		memcpy(JCOL22, &JCOL_22[0], NNZ22 * sizeof(int));
	}

	delete[] conNode;
}