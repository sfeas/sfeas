//Author		: Xiao Tuzi
//Date(Create)	: 08/17/2022
//Description	: this class is declaration of Math Basic Solver Library(MBSL)


#pragma once

#include "Global/GLobalVar.h"
#include <complex>
using zcompx = std::complex<double>;

class MathSolver {

public:
	void dcsrmv(const char* UPLO, double* A, int* iA, int* jA, const int m, const int n,
		const double* x, double* y, const double alpha = 1.0, const double beta = 0.0);
	/* y = beta*y + alpha* A * x, based on sparse BLAS
		A is an m-n sparse matrix in CSR format
		UPLO = 'U': Upper triangle storage for symmetric matrix(m=n)
		UPLO = 'L': Lower triangle storage for symmetric matrix(m=n)
		UPLO = 'F': Full storage for symmetric/asymmetric matrix
	*/

	void zcsrmv(const char* UPLO, zcompx* A, int nnz, int* iA, int* jA, const int m, const int n,
		const zcompx* x, zcompx* y, const zcompx alpha = { 1.0, 0.0 }, const zcompx beta = { 0.0, 0.0 });
	/* y = beta*y + alpha* A * x, based on sparse BLAS
		A is an m-n complex sparse matrix in CSR format
		UPLO = 'U': Upper triangle storage for symmetric matrix(m=n)
		UPLO = 'L': Lower triangle storage for symmetric matrix(m=n)
		UPLO = 'F': Full storage for symmetric/asymmetric matrix
	*/

	void dcsrmm(const char* UPLO, double* A, int* iA, int* jA, const double* B, double* C,
		const int m, const int k, const int n, const double alpha = 1.0, const double beta = 0.0);
	/* C = beta*C + alpha* A * B, based on sparse BLAS
		A is an m-k sparse matrix in CSR format, B is a k-n dense matrix, and C is an m-n dense matrix
		UPLO = 'U': Upper triangle storage for symmetric matrix(m=k)
		UPLO = 'L': Lower triangle storage for symmetric matrix(m=k)
		UPLO = 'F': Full storage for symmetric/asymmetric matrix
	*/

	void dgemv(const double* A, const int m, const int n, const double* x, double* y,
		const char* TRANSA = "N", const double alpha = 1.0, const double beta = 0.0);
	/* y = beta*y + alpha* op(A) * x, based on cblas
		op(A) is an m-n dense matrix
		TRANSA = "T":  op(A) = A.T
	*/

	void dgemm(const double* A, const double* B, double* C, const int m, const int k, const int n,
		const char* TRANSA = "N", const char* TRANSB = "N", const double alpha = 1.0, const double beta = 0.0);
	/* C = beta*C + alpha* op(A) * op(B), based on cblas
		op(A) is an m-k dense matrix, op(B) is a k-n dense matrix, and C is an m-n dense matrix
		TRANSA = "T":  op(A) = A.T
		TRANSB = "T":  op(B) = B.T
	*/

	void zgemv(const zcompx* A, const int m, const int n, const zcompx* x,
		zcompx* y, const char* TRANSA = "N", const double alpha = 1.0, const double beta = 0.0);
	/* y = beta*y + alpha* op(A) * x, based on cblas
		op(A) is an m-n dense matrix
		TRANSA = "T":  op(A) = A.T
	*/

	void zgemm(const zcompx* A, const zcompx* B, zcompx* C, const int m, const int k, const int n, 
		const char* TRANSA = "N", const char* TRANSB = "N", const double alpha = 1.0, const double beta = 0.0);
	/* C = beta*C + alpha* op(A) * op(B), based on cblas
		op(A) is an m-k dense matrix, op(B) is a k-n dense matrix, and C is an m-n dense matrix
		TRANSA = "T":  op(A) = A.T
		TRANSB = "T":  op(B) = B.T
	*/

	void dgesv(const double* A, const int n, double* x, const double* b);
	/* solve A x = b
		A is an n-n dense matrix
	*/

	void zgesv(const zcompx* A, const int n, zcompx* x, const zcompx* b);
	/* solve A x = b
		A is an n-n dense matrix in complex
	*/

	void decompose(double* A = GK, int* iA = IROW, int* jA = JCOL, int n = nEqu);
	/* solve A x = b, decompose A;
		A is an n-n sparse matrix in CSR format, and stored upper triangle part
	*/

	void substitute(int n = nEqu, double* b = RHS, double* x = U);
	/* solve A x = b, substitute b to get x;
		A is an n-n sparse matrix in CSR format, and stored upper triangle part
	*/

	void release();
	/* release the memory by decompose
	*/

	void DirectSolve(double* A = GK, int* iA = IROW, int* jA = JCOL, int n = nEqu, double* b = RHS, double* x = U);
	/* direct solve A x = b
		A is an n-n sparse matrix in CSR format, and stored upper triangle part
	*/

	void IterateSolve(double* A = GK, int* iA = IROW, int* jA = JCOL, int n = nEqu, double* b = RHS, double* x = U);
	/* iterative solve A x = b
		A is an n-n sparse matrix in CSR format, and stored upper triangle part
	*/

	void zDirectSolve(zcompx* A = CGK, int* iA = IROW, int* jA = JCOL, int n = nEqu, zcompx* b = CRHS, zcompx* x = CU);
	/* direct solve A x = b in complex
		A is an n-n sparse matrix in CSR format, and stored upper triangle part
	*/

	void zIterateSolve(zcompx* A = CGK, int* iA = IROW, int* jA = JCOL, int n = nEqu, zcompx* b = CRHS, zcompx* x = CU);
	/* iterative solve A x = b in complex
		A is an n-n sparse matrix in CSR format, and stored upper triangle part
	*/

	void eigenSolve(int n0, double Emin, double Emax, double* eigVal, double* eigVec, int n,
		double* A = GK, int* iA = IROW, int* jA = JCOL, double* B = GM, int* iB = IROW, int* jB = JCOL, int N = nEqu);
	/* solve K*x = ¦Ë*M*x 
		K, M are an n-n sparse matrix in CSR format, and stored upper triangle part
	*/

};

extern MathSolver MS;

