//Author		: Xiao Tuzi
//Date(Create)	: 08/19/2022
//Description	: this program is definition of Elem45


#include <numeric>

#include "Element/Elem45.h"
#include "Global/GlobalVar.h"
#include "Keyword/KETYPE.h"
#include "Keyword/KMAT.h"
#include "Keyword/KANTYPE.h"
#include "Keyword/KLUMPM.h"
#include "Timer/Timer.h"
#include "MathSolver/MathSolver.h"
#include "Initialize/ElemList.h"
#include "Initialize/MatList.h"
#include "DataType/Matrix.h"
#include "Element/Tools/getIndex.h"
#include "Element/Tools/shapeFunc45.h"
#include "Element/Tools/jacobiMat45.h"
#include "Element/Tools/getBMat45.h"
#include "Element/Tools/getNMat45.h"
#include "Element/Tools/calStrainExtraMat.h"

void Elem45::calElemMat(int iElem, bool isAssemble) {
	Timer timer("AssGlobalMat");

	int ETID = ELEM[iElem].ETypeID;
	int MatID = ELEM[iElem].MatID;

	// nElemNode: num of node of node
	// nNodeDof	: num of node dof
	// nIntPt	: num of integral point
	// nElemDof	: num of element dof
	// DENS		: // density
	// GE		: // material damping

	int nElemNode = ETYPE[ETID].nENode;
	int nNodeDof = ETYPE[ETID].nNodeDof;
	int nIntPt = ETYPE[ETID].nIntePoint;
	int nElemDof = nElemNode * nNodeDof;
	double DENS = MAT[MatID].DENS;
	double GE = MAT[MatID].GE;
	
	// ID		: element node array
	// XP		: guass integral point
	// WP		: guass integral weight
	// fshape	: shape function
	// Dshape	: shape function derivative for local coordinate
	// Dshp		: shape function derivative for global coordinate
	// BMat		: element matrix B to calculate ElemK
	// NMat		: shape function matrix to calculate ElemM
	// DBMat	: element matrix DBMat to calculate ElemK
	// ElemXYZ	: element node coordinate
	// detJ		: determinant of Jacobi Matrix
	// invJ		: inverse of Jacobi Matrix
	// ElemK	: element stiffness matrix
	// ElemM	: element mass matrix
	// ElemD	: element damp matrix

	int* ID = new int[nElemNode]();
	double* XP = new double[3]();
	double WP = 0.0;
	double* fshape = new double[nElemNode]();
	dMatrix Dshape(3, nElemNode);
	dMatrix Dshp(3, nElemNode);
	dMatrix BMat(6, nElemDof);
	dMatrix NMat(3, nElemDof);
	dMatrix DBMat(6, nElemDof);
	dMatrix ElemXYZ(nElemNode, 3);
	double detJ = 0.0;
	dMatrix invJ(3, 3);
	dMatrix ElemK(nElemDof, nElemDof);
	dMatrix ElemM;
	dMatrix ElemD;

	bool isMassMat, isDampMat;
	// confirm calculate which one of ElemMat
	if (ANTYPE->AnType == "STA") {
		isMassMat = false;
		isDampMat = false;
	}
	else {
		isMassMat = true;
		isDampMat = true;
		if (ANTYPE->AnType == "MOA") { isDampMat = false; }
	}
	if (isMassMat){ ElemM.setSize(nElemDof, nElemDof); }
	if (isDampMat){ ElemD.setSize(nElemDof, nElemDof); }
	
	// get ID array
	// 3D4 Tetra, first order
	if (nElemNode == 4) {
		ID[0] = ELEM[iElem].ENodeID[0];
		ID[1] = ELEM[iElem].ENodeID[1];
		ID[2] = ELEM[iElem].ENodeID[2];
		ID[3] = ELEM[iElem].ENodeID[4];
	}
	// 3D6 Wedge, first order
	else if (nElemNode == 6) {
		ID[0] = ELEM[iElem].ENodeID[0];
		ID[1] = ELEM[iElem].ENodeID[1];
		ID[2] = ELEM[iElem].ENodeID[2];
		ID[3] = ELEM[iElem].ENodeID[4];
		ID[4] = ELEM[iElem].ENodeID[5];
		ID[5] = ELEM[iElem].ENodeID[6];
	}
	// 3D8 Hexa, first order
	else if (nElemNode == 8) {
		for (int i = 0; i < nElemNode; i++) { ID[i] = ELEM[iElem].ENodeID[i]; }
	}

	// get ElemXYZ
	for (int i = 0; i < nElemNode; i++) {
		ElemXYZ(i, 0) = NODE[TNode[ID[i]]].NodeCoord[0];
		ElemXYZ(i, 1) = NODE[TNode[ID[i]]].NodeCoord[1];
		ElemXYZ(i, 2) = NODE[TNode[ID[i]]].NodeCoord[2];
	}

	// calculate ELemMat by integral Solu, K = B.T * D * B
	for (int iGP = 0; iGP < nIntPt; iGP++) {
		// get integral point(3D)
		XP[0] = ElemPtr[ETID].XP3(0, iGP);
		XP[1] = ElemPtr[ETID].XP3(1, iGP);
		XP[2] = ElemPtr[ETID].XP3(2, iGP);
		WP = ElemPtr[ETID].WP3[iGP];

		// calculate fshape and Dshape
		shapeFunc45(iGP, nElemNode, XP, fshape, Dshape);

		// calculate detJ and invJ
		jacobiMat45(nElemNode, ElemXYZ, Dshape, detJ, invJ);

		// calculate Dshp(Dshp = invJ * Dshape)
		MS.dgemm(invJ.datas(), Dshape.datas(), Dshp.datas(), 3, 3, nElemNode);

		// calculate BMat(strain - disp matrix)
		getBMat45(nElemNode, Dshp, BMat);

		// calculate DBMat(stress - strain matrix)
		//MS.dgemm(*DMat, *BMat, *DBMat, 6, 6, nElemDof);
		MS.dgemm(MatPtr[MatID].DMat.datas(), BMat.datas(), DBMat.datas(), 6, 6, nElemDof);

		// calculate NMat
		if (isMassMat) { getNMat45(nElemNode, fshape, NMat); }

		// calculate ElemK(ElemK = B.T * D * B)
		MS.dgemm(BMat.datas(), DBMat.datas(), ElemK.datas(), nElemDof, 6, nElemDof, "T", "N", detJ * WP, 1.0);

		// calculate ElemM(ElemM = DENS * N.T * N)
		if (isMassMat) {
			MS.dgemm(NMat.datas(), NMat.datas(), ElemM.datas(), nElemDof, 3, nElemDof, "T", "N", DENS * detJ * WP, 1.0);
		}

		// calculate ElemD(ElemD = GE * ElemK)
		if (isDampMat) {
			for (int i = 0; i < nElemDof; i++) {
				for (int j = i; j < nElemDof; j++) {
					ElemD(i, j) = GE * ElemK(i, j);
				}
			}
		}
	}	// end integral Solu

	// lumped mass matrix
	double summ;
	if (isMassMat && LUMPM->IsLumped == 1) {
		for (int i = 0; i < nElemDof; i++) {
			// diagonal elements are the sum of the row
			summ = 0.0;
			for (int j = 0; j < nElemDof; j++) { summ += ElemM(i, j); }
			ElemM(i, i) = summ;
			// off-diagonal elements are 0
			for (int j = 0; j < nElemDof; j++) {
				if (i != j) { ElemM(i, j) = 0.0; }
			}
		}
	}

	// release memory in advance
	delete[] XP;
	delete[] fshape;
	BMat.destroy();
	NMat.destroy();
	DBMat.destroy();
	ElemXYZ.destroy();
	Dshape.destroy();
	Dshp.destroy();
	invJ.destroy();

	// assemble element matrix to global matrix
	if (isAssemble) {
		std::vector<int> LM(nElemDof, 0);

		// get LM array
		int j;
		for (int i = 0; i < nElemNode; i++) {
			j = i * nNodeDof;
			for (int k = 0; k < nNodeDof; k++) { LM[j + k] = NStat[TNode[ID[i]]].DOF[k]; }
		}

		// assembly
		int count = -1;
		int IR, IC, IP;
		for (int i = 0; i < nElemDof; i++) {
			for (int j = i; j < nElemDof; j++) {
				count += 1;
				// storage upper triangle only(j >= i)
				if (LM[i] <= LM[j]) {
					IR = LM[i];
					IC = LM[j];
				}
				else {
					IR = LM[j];
					IC = LM[i];
				}

				// form GK11, GM11, GD11
				if (IR <= nEqu && IC <= nEqu) {
					if (getIndex(IR, IC, IROW, JCOL, nEqu, NNZ, IP)){
						GK[IP] += ElemK(i, j);
						if (isMassMat) { GM[IP] += ElemM(i, j); }
						if (isDampMat) { GD[IP] += ElemD(i, j); }
					}
				}
				// form GK12, GM12, GD12
				else if (IR <= nEqu && IC > nEqu) {
					IC = IC - nEqu;
					if (getIndex(IR, IC, IROW12, JCOL12, nEqu, NNZ12, IP)) {
						GK12[IP] += ElemK(i, j);
						if (isMassMat) { GM12[IP] += ElemM(i, j); }
						if (isDampMat) { GD12[IP] += ElemD(i, j); }
					}
				}
				// form GK22, GM22, GD22
				else if (IR > nEqu && IC > nEqu) {
					IR = IR - nEqu;
					IC = IC - nEqu;
					if (getIndex(IR, IC, IROW22, JCOL22, nEqu2, NNZ22, IP)) {
						GK22[IP] += ElemK(i, j);
						if (isMassMat) { GM22[IP] += ElemM(i, j); }
						if (isDampMat) { GD22[IP] += ElemD(i, j); }
					}
				}
			}
		}
	}

	// save ElemMat, storage upper triangle in 1D array by row(jc >= ir)
	bool saveElemMat = false;
	if (saveElemMat) {
		double ElemStifMat[300] = { 0.0 };	// element stiffness matrix
		double ElemMassMat[300] = { 0.0 };	// element mass matrix
		double ElemDampMat[300] = { 0.0 };	// element damp matrix
		int ij = -1;
		for (int ir = 0; ir < nElemDof; ir++) {
			for (int jc = ir; jc < nElemDof; jc++) {
				ij += 1;
				ElemStifMat[ij] = ElemK[ir, jc];
				if (isMassMat) { ElemMassMat[ij] = ElemM[ir, jc]; }
				if (isDampMat) { ElemDampMat[ij] = ElemD[ir, jc]; }
			}
		}

		// output  element matrix information

	}

	// release memory
	delete[] ID;
	ElemK.destroy();
	if (isMassMat) { ElemM.destroy(); }
	if (isDampMat) { ElemD.destroy(); }
}

void Elem45::calElemSTRA(int iElem) {
	int ETID = ELEM[iElem].ETypeID;

	// nElemNode: num of node of node
	// nNodeDof	: num of node dof
	// nIntPt	: num of integral point
	// nElemDof	: num of element dof

	int nElemNode = ETYPE[ETID].nENode;
	int nNodeDof = ETYPE[ETID].nNodeDof;
	int nIntPt = ETYPE[ETID].nIntePoint;
	int nElemDof = nElemNode * nNodeDof;

	// ID		: element node array
	// IDU		: element node array diaplacement
	// XP		: guass integral point
	// WP		: guass integral weight
	// fshape	: shape function
	// Dshape	: shape function derivative for local coordinate
	// Dshp		: shape function derivative for global coordinate
	// BMat		: element matrix B to calculate ElemK
	// NMat		: shape function matrix to calculate ElemM
	// DBMat	: element matrix DBMat to calculate ElemK
	// ElemXYZ	: element node coordinate
	// detJ		: determinant of Jacobi Matrix
	// invJ		: inverse of Jacobi Matrix
	// inteStrain: integral point Strain


	int* ID = new int[nElemNode]();
	double* IDU = new double[nElemDof]();
	double* XP = new double[3]();
	double WP = 0.0;
	double* fshape = new double[nElemNode]();
	dMatrix Dshape(3, nElemNode);
	dMatrix Dshp(3, nElemNode);
	dMatrix BMat(6, nElemDof);
	dMatrix NMat(3, nElemDof);
	dMatrix DBMat(6, nElemDof);
	dMatrix ElemXYZ(nElemNode, 3);
	double detJ = 0.0;
	dMatrix invJ(3, 3);
	double* inteStrain = new double[6]();

	// get ID array
	if (nElemNode == 4) {
		ID[0] = ELEM[iElem].ENodeID[0];
		ID[1] = ELEM[iElem].ENodeID[1];
		ID[2] = ELEM[iElem].ENodeID[2];
		ID[3] = ELEM[iElem].ENodeID[4];
	}
	else if (nElemNode == 6) {
		ID[0] = ELEM[iElem].ENodeID[0];
		ID[1] = ELEM[iElem].ENodeID[1];
		ID[2] = ELEM[iElem].ENodeID[2];
		ID[3] = ELEM[iElem].ENodeID[4];
		ID[4] = ELEM[iElem].ENodeID[5];
		ID[5] = ELEM[iElem].ENodeID[6];
	}
	else if (nElemNode == 8) {
		for (int i = 0; i < nElemNode; i++) { ID[i] = ELEM[iElem].ENodeID[i]; }
	}

	// get ElemXYZ
	for (int i = 0; i < nElemNode; i++) {
		ElemXYZ(i, 0) = NODE[TNode[ID[i]]].NodeCoord[0];
		ElemXYZ(i, 1) = NODE[TNode[ID[i]]].NodeCoord[1];
		ElemXYZ(i, 2) = NODE[TNode[ID[i]]].NodeCoord[2];
	}

	// get IDU array
	for (int i = 0; i < nElemNode; i++) {
		for (int j = 0; j < NStat[TNode[ID[i]]].NDOFN; j++) {
			IDU[nNodeDof * i + j] = NStat[TNode[ID[i]]].U[j];
		}
	}

	// calculate element strain of integral point by integral Solu, strain = BMat * IDU
	for (int iGP = 0; iGP < nIntPt; iGP++) {
		// get integral point(3D)
		XP[0] = ElemPtr[ETID].XP3(0, iGP);
		XP[1] = ElemPtr[ETID].XP3(1, iGP);
		XP[2] = ElemPtr[ETID].XP3(2, iGP);
		WP = ElemPtr[ETID].WP3[iGP];

		// calculate fshapeand Dshape
		shapeFunc45(iGP, nElemNode, XP, fshape, Dshape);

		// calculate detJ and invJ
		jacobiMat45(nElemNode, ElemXYZ, Dshape, detJ, invJ);

		// calculate Dshp(Dshp = invJ * Dshape)
		MS.dgemm(invJ.datas(), Dshape.datas(), Dshp.datas(), 3, 3, nElemNode);

		// calculate BMat(strain - disp matrix)
		getBMat45(nElemNode, Dshp, BMat);

		// calculate element strain of integral point, strain = BMat * IDU
		MS.dgemm(BMat.datas(), IDU, inteStrain, 6, nElemDof, 1);
		for (int i = 0; i < 6; i++) {
			EStat[iElem].STRA[i][iGP + 1] = inteStrain[i];
		}
	}

	// calculate element node strain
	// 3D4I1--constant strain element
	if (nElemNode == 4) {
		for (int i = 0; i < 6; i++) {
			for (int j = 1; j < 9; j++) {
				EStat[iElem].STRA[i][j] = EStat[iElem].STRA[i][1];
			}
		}
	}
	// 3D6I6
	else if (nElemNode == 6) {
		for (int i = 0; i < 6; i++) {
			for (int j = 1; j < 9; j++) {
				EStat[iElem].STRA[i][j] = EStat[iElem].STRA[i][1];
			}
		}
	}
	// 3D8I8
	else if (nElemNode == 8 && nIntPt == 8) {
		// strain extrapolation matrix
		dMatrix strainExtraMat(8, 8);

		// temp matrix
		dMatrix temp(6, 8);
		dMatrix temp1(6, 8);

		calStrainExtraMat(strainExtraMat);
		for (int i = 0; i < 6; i++) {
			memcpy(&temp(i, 0), &EStat[iElem].STRA[i][1], 8 * sizeof(double));
		}
		MS.dgemm(temp.datas(), strainExtraMat.datas(), temp1.datas(), 6, 8, 8, "N", "T");
		for (int i = 0; i < 6; i++) {
			memcpy(&EStat[iElem].STRA[i][1], &temp1(i, 0), 8 * sizeof(double));
		}
	}

	// calculate average strain(as element strain)
	for (int i = 0; i < 6; i++) {
		EStat[iElem].STRA[i][0] = std::accumulate(&EStat[iElem].STRA[i][1], &EStat[iElem].STRA[i][1]+8, 0.0);
	}

	// release memory
	delete[] ID;
	delete[] IDU;
	delete[] XP;
	delete[] fshape;
	delete[] inteStrain;

	return;
}

void Elem45::calElemSTRS(int iElem) {
	int MatID = ELEM[iElem].MatID;

	// Calculate element node strain first
	this->calElemSTRA(iElem);

	// Calculate element node stress
	MS.dgemm(MatPtr[MatID].DMat.datas(), &EStat[iElem].STRA[0][0], &EStat[iElem].STRE[0][0], 6, 6, 9);

	return;
}