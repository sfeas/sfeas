//Author		: Xiao Tuzi
//Date(Create)	: 01/09/2023
//Description	: this program is definition of getIndex function

#include "Element/Tools/getIndex.h"

bool getIndex(int iRow, int jCol, int* iA, int* jA, int n, int nnz, int& index) {
	// get index from sparse matrix by A(iRow, jCol)
	// iA:	rowIndex, n is length
	// jA:	colIndex, nnz is length
	// return true: get index successfully, false: not find, means A(iRow, jCol) = 0.0

	for (int IP = iA[iRow - 1]; IP < iA[iRow]; IP++) {
		if (jCol == jA[IP - 1]) {
			index = IP - 1;
			return true;
		}
	}
	return false;
}
