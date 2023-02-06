//Author		: Xiao Tuzi
//Date(Create)	: 08/17/2022
//Description	: this class is definition of Math Basic Solver Library(MBSL), based on Intel MKL


#include "MathSolver/MathSolver.h"
#include "Global/Files.h"
#include "Keyword/KANTYPE.h"
#include "Keyword/KEQUSLV.h"
#include "Keyword/KEIGSLV.h"
#include "Print/PrintMsg.h"
#include "Timer/Timer.h"

#include "mkl_cblas.h"
#include "mkl_spblas.h"
#include "mkl_lapacke.h"
#include "mkl_pardiso.h"
#include "mpi.h"
#include "dmumps_c.h"
#include "zmumps_c.h"
#include "mkl_solvers_ee.h"
#include <Eigen\SparseCore>
#include <Spectra/SymGEigsShiftSolver.h>
#include <Spectra/MatOp/SymShiftInvert.h>
#include <Spectra/MatOp/DenseSymMatProd.h>
#include <Spectra/MatOp/SparseSymMatProd.h>
#include "catch.hpp"

using namespace Spectra;
using Matrix = Eigen::MatrixXd;
using Vector = Eigen::VectorXd;
using SpMat = Eigen::SparseMatrix<double>;
using OpType = SymShiftInvert<double, Eigen::Sparse, Eigen::Sparse>;
using BOpType = SparseSymMatProd<double>;
#define USE_COMM_WORLD -987654

// function definition
Eigen::SparseMatrix<double, Eigen::RowMajor> csr2SpMat(double* A, int* iA, int* jA, int N, int NNZ) {
	// change CSR format to SpMat format by triplets format
	/*  CSR format:     iA[N+1], jA[NNZ], A[NNZ], upper triangle
		Triplet format: iAA[NNZ], jA[NNZ], A[NNZ]
	*/
	std::vector<Eigen::Triplet<double>> triplets;
	Eigen::SparseMatrix<double, Eigen::RowMajor> A0;

	A0.resize(N, N);

	// get iAA from iA
	int* iAA = new int[NNZ] {};
	int mi;         // mi elemets of i-row
	int count = 0;
	int irow = 1;
	for (int i = 0; i < N; ++i) {
		mi = iA[i + 1] - iA[i];
		if (mi > 0) {
			for (int j = 0; j < mi; ++j) {
				iAA[count] = irow;
				count++;
			}
		}
		else {
			iAA[count] = iA[i];
			count++;
		}
		irow++;
	}

	// get triplets from iAA, jA, A
	for (int i = 0; i < NNZ; i++) {
		triplets.push_back(Eigen::Triplet<double>(iAA[i] - 1, jA[i] - 1, A[i]));
	}
	// get SpMat from triplets
	A0.setFromTriplets(triplets.begin(), triplets.end());
	A0.makeCompressed();

	// Eigen solver only uses the lower triangle
	A0 = A0.transpose();

	delete[] iAA;
	return A0;
}


void MathSolver::dcsrmv(const char* UPLO, double* A, int* iA, int* jA,
	const int m, const int n, const double* x, double* y, const double alpha, const double beta) {
	
	// Sparse BLAS IE variables
	sparse_matrix_t A0 = nullptr;		// Handle containing sparse matrix in internal data structure
	struct matrix_descr descrA;			// Structure specifying sparse matrix properties

	// Create handle for matrix stored in CSR format
	if (*UPLO == 'U') {
		descrA.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrA.mode = SPARSE_FILL_MODE_UPPER;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else if (*UPLO == 'L') {
		descrA.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrA.mode = SPARSE_FILL_MODE_LOWER;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else if (*UPLO == 'F') {
		descrA.type = SPARSE_MATRIX_TYPE_GENERAL;
		descrA.mode = SPARSE_FILL_MODE_FULL;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else {
		printMsg.error("MathSolver::dcsrmv(): UPLO is wrong");
	}
	mkl_sparse_d_create_csr(&A0, SPARSE_INDEX_BASE_ONE, m, n, iA, iA + 1, jA, A);
	mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A0, descrA, x, beta, y);
	mkl_sparse_destroy(A0);

}

void MathSolver::zcsrmv(const char* UPLO, zcompx* A, int nnz, int* iA, int* jA,
	const int m, const int n, const zcompx* x, zcompx* y, const zcompx alpha, const zcompx beta) {

	// Sparse BLAS IE variables
	sparse_matrix_t A0 = nullptr;		// Handle containing sparse matrix in internal data structure
	struct matrix_descr descrA;			// Structure specifying sparse matrix properties

	MKL_Complex16* A00 = new MKL_Complex16[nnz]();
	memcpy(A00, A, nnz * sizeof(zcompx));

	MKL_Complex16* x00 = new MKL_Complex16[n]();
	memcpy(x00, x, n * sizeof(zcompx));

	MKL_Complex16* y00 = new MKL_Complex16[m]();
	memcpy(y00, y, m * sizeof(zcompx));

	MKL_Complex16 alpha00, beta00;
	alpha00.imag = alpha.imag();
	alpha00.real = alpha.real();
	beta00.imag = beta.imag();
	beta00.real = beta.real();

	// Create handle for matrix stored in CSR format
	if (*UPLO == 'U') {
		descrA.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrA.mode = SPARSE_FILL_MODE_UPPER;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else if (*UPLO == 'L') {
		descrA.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrA.mode = SPARSE_FILL_MODE_LOWER;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else if (*UPLO == 'F') {
		descrA.type = SPARSE_MATRIX_TYPE_GENERAL;
		descrA.mode = SPARSE_FILL_MODE_FULL;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else {
		printMsg.error("MathSolver::dcsrmv(): UPLO is wrong");
	}

	mkl_sparse_z_create_csr(&A0, SPARSE_INDEX_BASE_ONE, m, n, iA, iA + 1, jA, A00);
	mkl_sparse_z_mv(SPARSE_OPERATION_NON_TRANSPOSE, alpha00, A0, descrA, x00, beta00, y00);
	mkl_sparse_destroy(A0);

	memcpy(y, y00, m * sizeof(zcompx));

	delete[] A00;
	delete[] x00;
	delete[] y00;
}

void MathSolver::dcsrmm(const char* UPLO, double* A, int* iA, int* jA, const double* B, double* C,
	const int m, const int k, const int n, const double alpha, const double beta) {

	// Sparse BLAS IE variables
	sparse_matrix_t A0 = nullptr;		// Handle containing sparse matrix in internal data structure
	struct matrix_descr descrA;			// Structure specifying sparse matrix properties

	// Create handle for matrix stored in CSR format
	if (*UPLO == 'U') {
		descrA.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrA.mode = SPARSE_FILL_MODE_UPPER;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else if (*UPLO == 'L') {
		descrA.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrA.mode = SPARSE_FILL_MODE_LOWER;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else if (*UPLO == 'F') {
		descrA.type = SPARSE_MATRIX_TYPE_GENERAL;
		descrA.mode = SPARSE_FILL_MODE_FULL;
		descrA.diag = SPARSE_DIAG_NON_UNIT;
	}
	else {
		printMsg.error("MathSolver::dcsrmm(): UPLO is wrong");
	}
	mkl_sparse_d_create_csr(&A0, SPARSE_INDEX_BASE_ONE, m, n, iA, iA + 1, jA, A);
	mkl_sparse_d_mm(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A0, descrA, SPARSE_LAYOUT_ROW_MAJOR, B, n, k, beta, C, m);
	mkl_sparse_destroy(A0);
}

void MathSolver::dgemv(const double* A, const int m, const int n, const double* x, double* y,
	const char* TRANSA, const double alpha, const double beta) {

	const enum CBLAS_LAYOUT Order = CblasRowMajor;
	if (*TRANSA == 'T') {
		const enum CBLAS_TRANSPOSE Trans = CblasTrans;
		int incx = 1;
		int incy = 1;
		cblas_dgemv(Order, Trans, n, m, alpha, A, m, x, incx, beta, y, incy);
	}
	else if (*TRANSA == 'N') {
		const enum CBLAS_TRANSPOSE Trans = CblasNoTrans;
		int incx = 1;
		int incy = 1;
		cblas_dgemv(Order, Trans, m, n, alpha, A, n, x, incx, beta, y, incy);
	}
	else {
		printMsg.error("MathSolver::dgemv(): TRANSA is wrong");
	}
}

void MathSolver::dgemm(const double* A, const double* B, double* C, const int m, const int k, const int n,
	const char* TRANSA, const char* TRANSB, const double alpha, const double beta) {

	const enum CBLAS_LAYOUT Order = CblasRowMajor;
	if (*TRANSA != 'T' && *TRANSA != 'N') {
		printMsg.error("MathSolver::dgemm(): TRANSA is wrong");
	}
	else if (*TRANSB != 'T' && *TRANSB != 'N') {
		printMsg.error("MathSolver::dgemm(): TRANSB is wrong");
	}
	else if (*TRANSA == 'N' && *TRANSB == 'N') {
		const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
		cblas_dgemm(Order, TransA, TransB, m, n, k, alpha, A, k, B, n, beta, C, n);
	}
	else if (*TRANSA == 'T' && *TRANSB == 'N') {
		const enum CBLAS_TRANSPOSE TransA = CblasTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
		cblas_dgemm(Order, TransA, TransB, m, n, k, alpha, A, m, B, n, beta, C, n);
	}
	else if (*TRANSA == 'N' && *TRANSB == 'T') {
		const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasTrans;
		cblas_dgemm(Order, TransA, TransB, m, n, k, alpha, A, k, B, k, beta, C, n);
	}
	else if (*TRANSA == 'T' && *TRANSB == 'T') {
		const enum CBLAS_TRANSPOSE TransA = CblasTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasTrans;
		cblas_dgemm(Order, TransA, TransB, m, n, k, alpha, A, k, B, k, beta, C, n);
	}
}

void MathSolver::zgemv(const zcompx* A, const int m, const int n, const zcompx* x,
	zcompx* y, const char* TRANSA, const double alpha, const double beta) {
	const enum CBLAS_LAYOUT Order = CblasRowMajor;
	if (*TRANSA == 'T') {
		const enum CBLAS_TRANSPOSE Trans = CblasTrans;
		int incx = 1;
		int incy = 1;
		cblas_zgemv(Order, Trans, n, m, &alpha, A, m, x, incx, &beta, y, incy);
	}
	else if (*TRANSA == 'N') {
		const enum CBLAS_TRANSPOSE Trans = CblasNoTrans;
		int incx = 1;
		int incy = 1;
		cblas_zgemv(Order, Trans, m, n, &alpha, A, n, x, incx, &beta, y, incy);
	}
	else {
		printMsg.error("MathSolver::zgemv(): TRANSA is wrong");
	}
}

void MathSolver::zgemm(const zcompx* A, const zcompx* B, zcompx* C, const int m, const int k, const int n, 
	const char* TRANSA, const char* TRANSB, const double alpha, const double beta) {

	const enum CBLAS_LAYOUT Order = CblasRowMajor;
	if (*TRANSA != 'T' && *TRANSA != 'N') {
		printMsg.error("MathSolver::zgemm(): TRANSA is wrong");
	}
	else if (*TRANSB != 'T' && *TRANSB != 'N') {
		printMsg.error("MathSolver::zgemm(): TRANSB is wrong");
	}
	else if (*TRANSA == 'N' && *TRANSB == 'N') {
		const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
		cblas_zgemm(Order, TransA, TransB, m, n, k, &alpha, A, k, B, n, &beta, C, n);
	}
	else if (*TRANSA == 'T' && *TRANSB == 'N') {
		const enum CBLAS_TRANSPOSE TransA = CblasTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
		cblas_zgemm(Order, TransA, TransB, m, n, k, &alpha, A, m, B, n, &beta, C, n);
	}
	else if (*TRANSA == 'N' && *TRANSB == 'T') {
		const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasTrans;
		cblas_zgemm(Order, TransA, TransB, m, n, k, &alpha, A, k, B, k, &beta, C, n);
	}
	else if (*TRANSA == 'T' && *TRANSB == 'T') {
		const enum CBLAS_TRANSPOSE TransA = CblasTrans;
		const enum CBLAS_TRANSPOSE TransB = CblasTrans;
		cblas_zgemm(Order, TransA, TransB, m, n, k, &alpha, A, k, B, k, &beta, C, n);
	}
}

void MathSolver::dgesv(const double* A, const int n, double* x, const double* b) {
	Timer timerSolver("EquaSolver");

	int nrhs = 1;
	int lda = n;
	int ldb = nrhs;
	int* ipiv = new int[n];
	int info;

	double* A0 = new double[n * n]();
	double* b0 = new double[n]();
	memcpy(A0, A, n * n * sizeof(double));
	memcpy(b0, b, n * sizeof(double));
	info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, n, nrhs, A0, lda, ipiv, b0, ldb);
	if (info != 0) {
		printMsg.error("MathSolver::dgesv(): error code is", info);
	}
	else {
		memcpy(x, b0, n * sizeof(double));
	}
	delete[] A0;
	delete[] b0;
	delete[] ipiv;
}

void MathSolver::zgesv(const zcompx* A, const int n, zcompx* x, const zcompx* b) {
	Timer timerSolver("EquaSolver");

	int nrhs = 1;
	int lda = n;
	int ldb = nrhs;
	int* ipiv = new int[n];
	int info;

	MKL_Complex16* A0 = new MKL_Complex16[n * n]();
	MKL_Complex16* b0 = new MKL_Complex16[n]();
	memcpy(A0, A, n * n * sizeof(zcompx));
	memcpy(b0, b, n * sizeof(zcompx));
	info = LAPACKE_zgesv(LAPACK_ROW_MAJOR, n, nrhs, A0, lda, ipiv, b0, ldb);
	if (info != 0) {
		printMsg.error("MathSolver::dgesv(): error code is", info);
	}
	else {
		memcpy(x, b0, n * sizeof(zcompx));
	}
	delete[] A0;
	delete[] b0;
	delete[] ipiv;
}

void MathSolver::decompose(double* A, int* iA, int* jA, int n) {
	Timer timerSolver("EquaSolver");

	if (EQUSLV->SolverType == "PDS") {
		// pardiso solver
		int mtype = 2;				// Matrix type, 2 - Real and symmetric positive definite
		int nrhs = 1;				// Number of right hand sides
		void* pt[64] = { 0 };		// Internal solver memory pointer
		int iparm[64] = { 0 };		// Pardiso control parameters
		int maxfct = 1;				// Maximum number of numerical factorizations
		int mnum = 1;				// Which factorization to use
		int msglvl = 0;				// = 0: No print statistical information in file
		int error = 0;				// Initialize error flag
		int phase;					// Controls the execution of the solver
		double ddum;				// Double dummy
		int idum;					// Integer dummy
		const char* dirname = CurrentDirectory.data(); // The routine creates handle.pds in the directory

		pardiso_handle_delete(dirname, &error);	// delete files if exist

		// reordering and factorization
		phase = 12;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, A, iA, jA, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
		if (error != 0) {
			printMsg.error("MathSolver::decompose(): parsido factorization error", error);
		}

		// store internal structures from pardiso to a file.
		pardiso_handle_store(pt, dirname, &error);
		if (error != 0) {
			printMsg.error("MathSolver::decompose(): parsido save error ", error);
		}
		// release internal memory
		phase = -1;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, &ddum, iA, jA, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
	}
	else if (EQUSLV->SolverType == "MPS") {
		// mumps solver
		DMUMPS_STRUC_C id;
		MUMPS_INT8 nnz = iA[n] - iA[0];
		MUMPS_INT* irn = new MUMPS_INT[NNZ]{};

		int myid;
		MPI_Init(nullptr, nullptr);
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);

		// initialize(job = -1)
		id.comm_fortran = USE_COMM_WORLD;
		id.par = 1;
		id.sym = 2;   // general symmetric
		id.job = -1;
		dmumps_c(&id);
		if (myid == 0) {
			// get irn from iA
			int mi;     // mi elemets of i-row
			int count = 0;
			int irow = 1;
			for (int i = 0; i < n; ++i) {
				mi = iA[i + 1] - iA[i];
				if (mi > 0) {
					for (int j = 0; j < mi; ++j) {
						irn[count] = irow;
						count++;
					}
				}
				else {
					irn[count] = iA[i];
					count++;
				}
				irow++;
			}

			id.n = n;
			id.nnz = nnz;
			id.irn = irn;
			id.jcn = jA;
			id.a = A;
		}
		else {
			printMsg.error("MathSolver::decompose(): mumps initialize error ");
		}

		// set output information
		id.icntl[0] = -1;
		id.icntl[1] = -1;
		id.icntl[2] = -1;
		id.icntl[3] = 0;

		// delete saved files if exist(job = -3)
		strcpy(id.save_prefix, "csave_restore");
		strcpy(id.save_dir, CurrentDirectory.data());
		id.job = -3;
		dmumps_c(&id);

		// analyse and factorization(job = 4)
		id.job = 4;
		dmumps_c(&id);
		if (myid != 0) {
			printMsg.error("MathSolver::decompose(): mumps factorization error ");
		}

		// save(job = 7)
		strcpy(id.save_prefix, "csave_restore");
		strcpy(id.save_dir, CurrentDirectory.data());
		id.job = 7;
		dmumps_c(&id);
		if (id.infog[0] < 0) {
			printMsg.error("MathSolver::decompose(): mumps save error ");
		}

		// terminate(job = -2)
		id.job = -2;
		dmumps_c(&id);

		MPI_Finalize();
	}
	else {
		printMsg.error("MathSolver::decompose() : SolverType error ");
	}
}

void MathSolver::substitute(int n, double* b, double* x) {
	Timer timerSolver("EquaSolver");

	if (EQUSLV->SolverType == "PDS") {
		// pardiso solver
		int mtype = 2;				// Matrix type, 2 - Real and symmetric positive definite
		int nrhs = 1;				// Number of right hand sides
		void* pt[64] = { 0 };		// Internal solver memory pointer
		int iparm[64] = { 0 };		// Pardiso control parameters
		int maxfct = 1;				// Maximum number of numerical factorizations
		int mnum = 1;				// Which factorization to use
		int msglvl = 0;				// = 0: No print statistical information in file
		int error = 0;				// Initialize error flag
		int phase;					// Controls the execution of the solver
		double ddum;				// Double dummy
		int idum;					// Integer dummy
		const char* dirname = CurrentDirectory.data(); // The routine creates handle.pds in the directory

		// restore pardiso internal structures from a file.
		pardiso_handle_restore(pt, dirname, &error);
		if (error != 0) {
			printMsg.error("MathSolver::substitute(): parsido restore error ", error);
		}

		// only back substitution solve and iterative refinement
		phase = 33;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, &ddum, &idum, &idum, &idum, &nrhs, iparm, &msglvl, b, x, &error);
		if (error != 0) {
			printMsg.error("MathSolver::substitute(): parsido substitute error ", error);
		}

		// release internal memory
		phase = -1;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, &ddum, &idum, &idum, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
	}
	else if (EQUSLV->SolverType == "MPS") {
		//mumps solver
		DMUMPS_STRUC_C id;

		int myid;
		MPI_Init(nullptr, nullptr);
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);

		// initialize(job = -1)
		id.comm_fortran = USE_COMM_WORLD;
		id.par = 1;
		id.sym = 2;   // general symmetric
		id.job = -1;
		dmumps_c(&id);
		if (myid == 0) {
		    id.rhs = b;
		    id.n = n;
		}
		else{
			printMsg.error("MathSolver::substitute(): mumps initialize error ");
		}
		// set output information
		id.icntl[0] = -1;
		id.icntl[1] = -1;
		id.icntl[2] = -1;
		id.icntl[3] = 0;

		// restore(job = 8)
		strcpy(id.save_prefix, "csave_restore");
		strcpy(id.save_dir, CurrentDirectory.data());
		id.job = 8;
		dmumps_c(&id);
		if (id.infog[0] < 0) {
			printMsg.error("MathSolver::substitute(): mumps restore error ");
		}

		// back substitution(job = 3)
		id.job = 3;
		dmumps_c(&id);
		if (id.infog[0] < 0) {
			printMsg.error("MathSolver::substitute(): mumps substitute error ");
		}
		else {
		    memcpy(x, id.rhs, n * sizeof(double));
		}

		// terminate(job = -2)
		id.job = -2;
		dmumps_c(&id);

		MPI_Finalize();	
	}
	else {
		printMsg.error("MathSolver::substitute() : SolverType error ");
	}
}

void MathSolver::release() {	
	Timer timerSolver("EquaSolver");

	if (EQUSLV->SolverType == "PDS") {
		// pardiso solver
		int error = 0;				// Initialize error flag
		const char* dirname = CurrentDirectory.data(); // The routine creates handle.pds in the directory

		// delete files with pardiso internal structure data
		pardiso_handle_delete(dirname, &error);
		if (error != 0) {
			printMsg.error("MathSolver::release(): pardiso release error: ", error);
		}
	}
	else if (EQUSLV->SolverType == "MPS") {
		// mumps solver
		DMUMPS_STRUC_C id;

		int myid;
		MPI_Init(nullptr, nullptr);
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);

		// initialize(job = -1)
		id.comm_fortran = USE_COMM_WORLD;
		id.par = 1;
		id.sym = 2;   // general symmetric
		id.job = -1;
		dmumps_c(&id);

		// set output information
		id.icntl[0] = -1;
		id.icntl[1] = -1;
		id.icntl[2] = -1;
		id.icntl[3] = 0;

		// delete saved files(job = -3)
		strcpy(id.save_prefix, "csave_restore");
		strcpy(id.save_dir, CurrentDirectory.data());
		id.job = -3;
		dmumps_c(&id);
		if (id.infog[0] < 0) {
			printMsg.error("MathSolver::release(): mumps release error: ");
		}

		// terminate(job = -2)
		id.job = -2;
		dmumps_c(&id);

		MPI_Finalize();
	}
	else {
		printMsg.error("MathSolver::release() : SolverType error ");
	}
}

void MathSolver::DirectSolve(double* A, int* iA, int* jA, int n, double* b, double* x) {
	Timer timerSolver("EquaSolver");

	if (EQUSLV->SolverType == "PDS") {
		// pardiso solver
		int mtype = 2;				// Matrix type, 2 - Real and symmetric positive definite
		int nrhs = 1;				// Number of right hand sides
		void* pt[64] = { 0 };		// Internal solver memory pointer
		int iparm[64] = { 0 };		// Pardiso control parameters
		int maxfct = 1;				// Maximum number of numerical factorizations
		int mnum = 1;				// Which factorization to use
		int msglvl = 0;				// = 0: No print statistical information in file
		int error = 0;				// Initialize error flag
		int phase;					// Controls the execution of the solver
		double ddum;				// Double dummy
		int idum;					// Integer dummy

		// direct solve
		phase = 13;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, A, iA, jA, &idum, &nrhs, iparm, &msglvl, b, x, &error);
		if (error != 0) {
			printMsg.error("MathSolver::DirectSolve(): pardiso solve error ", error);
		}

		// release internal memory
		phase = -1;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, &ddum, iA, jA, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
	}
	else if (EQUSLV->SolverType == "MPS") {
		// mumps solver
		DMUMPS_STRUC_C id;
		MUMPS_INT8 nnz = iA[n] - iA[0];
		MUMPS_INT* irn = new MUMPS_INT[NNZ]{};

		int myid;
		MPI_Init(nullptr, nullptr);
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);

		// initialize(job = -1)
		id.comm_fortran = USE_COMM_WORLD;
		id.par = 1;
		id.sym = 2;   // general symmetric
		id.job = -1;
		dmumps_c(&id);
		if (myid == 0) {
			// get irn from iA
			int mi;     // mi elemets of i-row
			int count = 0;
			int irow = 1;
			for (int i = 0; i < n; ++i) {
				mi = iA[i + 1] - iA[i];
				if (mi > 0) {
					for (int j = 0; j < mi; ++j) {
						irn[count] = irow;
						count++;
					}
				}
				else {
					irn[count] = iA[i];
					count++;
				}
				irow++;
			}

			id.n = n;
			id.nnz = nnz;
			id.irn = irn;
			id.jcn = jA;
			id.a = A;
			id.rhs = b;
		}
		else {
			printMsg.error("MathSolver::DirectSolve(): mumps initialize error ");
		}

		// set output information
		id.icntl[0] = -1;
		id.icntl[1] = -1;
		id.icntl[2] = -1;
		id.icntl[3] = 0;

		// analyse, factorization and solve(job = 6)
		id.job = 6;
		dmumps_c(&id);
		if (id.infog[0] < 0) {
			printMsg.error("MathSolver::DirectSolve(): mumps solve error ");
		}
		else {
			memcpy(x, id.rhs, n * sizeof(double));
		}

		// terminate(job = -2)
		id.job = -2;
		dmumps_c(&id);

		MPI_Finalize();
		delete[] irn;
	}
	else {
		printMsg.error("MathSolver::DirectSolve() : SolverType error ");
	}
}

void MathSolver::IterateSolve(double* A, int* iA, int* jA, int n, double* b, double* x) {

}

void MathSolver::zDirectSolve(zcompx* A, int* iA, int* jA, int n, zcompx* b, zcompx* x) {
	Timer timerSolver("EquaSolver");

	if (EQUSLV->SolverType == "PDS") {
		// pardiso solver
		int mtype = 6;				// Matrix type, 6 - Complex and symmetric positive definite
		int nrhs = 1;				// Number of right hand sides
		void* pt[64] = { 0 };		// Internal solver memory pointer
		int iparm[64] = { 0 };		// Pardiso control parameters
		int maxfct = 1;				// Maximum number of numerical factorizations
		int mnum = 1;				// Which factorization to use
		int msglvl = 0;				// = 0: No print statistical information in file
		int error = 0;				// Initialize error flag
		int phase;					// Controls the execution of the solver
		zcompx ddum;				// Double dummy
		int idum;					// Integer dummy

		// direct solve
		phase = 13;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, A, iA, jA, &idum, &nrhs, iparm, &msglvl, b, x, &error);
		if (error != 0) {
			printMsg.error("MathSolver::zDirectSolve(): pardiso solve error ");
		}
		// release internal memory
		phase = -1;
		pardiso(pt, &maxfct, &mnum, &mtype, &phase,
			&n, &ddum, iA, jA, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
	}
	else if(EQUSLV->SolverType == "MPS") {
		// mumps solver
		ZMUMPS_STRUC_C id;
		MUMPS_INT8 nnz = iA[n] - iA[0];
		MUMPS_INT* irn = new MUMPS_INT[NNZ]{};

		int myid;
		MPI_Init(nullptr, nullptr);
		MPI_Comm_rank(MPI_COMM_WORLD, &myid);

		// initialize(job = -1)
		id.comm_fortran = USE_COMM_WORLD;
		id.par = 1;
		id.sym = 2;   // general symmetric
		id.job = -1;
		zmumps_c(&id);
		if (myid == 0) {
			// get irn from iA
			int mi;     // mi elemets of i-row
			int count = 0;
			int irow = 1;
			for (int i = 0; i < n; ++i) {
				mi = iA[i + 1] - iA[i];
				if (mi > 0) {
					for (int j = 0; j < mi; ++j) {
						irn[count] = irow;
						count++;
					}
				}
				else {
					irn[count] = iA[i];
					count++;
				}
				irow++;
			}

			id.n = n;
			id.nnz = nnz;
			id.irn = irn;
			id.jcn = jA;
			id.a = new mumps_double_complex[nnz]();
			memcpy(id.a, A, nnz * sizeof(zcompx));
			id.rhs = new mumps_double_complex[n]();
			memcpy(id.rhs, b, n * sizeof(zcompx));
			//for (int i = 0; i < nnz; i++){
			//	id.a[i].r = A[i].real();
			//	id.a[i].i = A[i].imag();
			//}
			//id.rhs = new mumps_double_complex[n]();
			//for (int i = 0; i < n; i++) {
			//	id.rhs[i].r = b[i].real();
			//	id.rhs[i].i = b[i].imag();
			//}
			
			//id.a->r = A->real();
			//id.a->i= A->imag();
			//id.rhs->r = b->real();
			//id.rhs->i = b->imag();
		}
		else {
			printMsg.error("MathSolver::zDirectSolve(): mumps initialize error ");
		}

		// set output information
		id.icntl[0] = -1;
		id.icntl[1] = -1;
		id.icntl[2] = -1;
		id.icntl[3] = 0;

		// analyse, factorization and solve(job = 6)
		id.job = 6;
		zmumps_c(&id);
		if (id.infog[0] < 0) {
			printMsg.error("MathSolver::zDirectSolve(): mumps solve error ");
		}
		else {
			memcpy(x, id.rhs, n * sizeof(zcompx));
		}

		// terminate(job = -2)
		id.job = -2;
		zmumps_c(&id);

		MPI_Finalize();
		delete[] irn;
		delete[] id.a;
		delete[] id.rhs;
	}
	else {
		printMsg.error("MathSolver::zDirectSolve() : SolverType error ");
	}
}

void MathSolver::zIterateSolve(zcompx* A, int* iA, int* jA, int n, zcompx* b, zcompx* x) {

}

void MathSolver::eigenSolve(int n0, double Emin, double Emax, double* eigVal, double* eigVec, int m,
	double* A, int* iA, int* jA, double* B, int* iB, int* jB, int n) {
	Timer timerSolver("EquaSolver");
	
	if (EIGSLV->SolverType == "FST") {
		// feast eigen solver
		int m0 = 5 * n0;		// Search subspace dimension
		int mm;					// Eigenvalues found in range of [Emin, EMax]
		double* E = new double[m0]();
		double* X = new double[n * m0]();

		char uplo = 'U';
		int Solu = 0;
		int info = 0;
		int fpm[128];
		double epsout = 0.0;
		double* res = new double[m0]();

		feastinit(fpm);
		fpm[0] = 0;    // print statistical information
		dfeast_scsrgv(&uplo, &n, A, iA, jA, B, iB, jB, fpm, &epsout, &Solu, &Emin, &Emax, &m0, E, X, &mm, res, &info);

		if (info == 0) {
			m = (n0 > mm ? mm : n0);
			memcpy(eigVal, E, m * sizeof(double));
			memcpy(eigVec, X, n * m * sizeof(double));
		}
		else if (info == 1) {
			printMsg.error("MathSolver::eigenSolve(): feast solver error, no eigenvalues found in the search interval", info);
		}
		else if (info == 3) {
			printMsg.error("MathSolver::eigenSolve(): feast solver error, size subspace m0 too small(m0 < m), or upper frequency too large ", info);
		}
		else {
			printMsg.error("MathSolver::eigenSolve(): feast solver error, error in eigen solver ", info);
		}

		delete[] E;
		delete[] X;
		delete[] res;
	}
	else if (EIGSLV->SolverType == "MKL") {
		// mkl sparse solver
		char which = 'S';   // 'L' - largest (algebraic) eigenvalues,
		// 'S' - smallest (algebraic) eigenvalues
		int pm[128];
		double* res = new double[n0]();      // Residual
		int info;          // Errors

		// Sparse BLAS IE variables
		sparse_matrix_t K = nullptr, M = nullptr; // Handle containing sparse matrix in internal data structure
		struct matrix_descr descrK, descrM; // Structure specifying sparse matrix properties

		// Create handle for matrix stored in CSR format
		descrK.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrK.mode = SPARSE_FILL_MODE_UPPER;
		descrK.diag = SPARSE_DIAG_NON_UNIT;
		mkl_sparse_d_create_csr(&K, SPARSE_INDEX_BASE_ONE, n, n, iA, iA + 1, jA, A);

		descrM.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descrM.mode = SPARSE_FILL_MODE_UPPER;
		descrM.diag = SPARSE_DIAG_NON_UNIT;
		mkl_sparse_d_create_csr(&M, SPARSE_INDEX_BASE_ONE, n, n, iB, iB + 1, jB, B);

		// initialize input values
		mkl_sparse_ee_init(pm);

		//pm[1] = 2;          // Set tolerance
		//pm[2] = 1;          // the Krylov-Schur method

		// Solve the generized A*x = lamda*B*x eigenvalue problem
		info = mkl_sparse_d_gv(&which, pm, K, descrK, M, descrM, n0, &m, eigVal, eigVec, res);

		if (info != 0) {
			printMsg.error("MathSolver::eigenSolve(): mkl sparse solver error ", info);
		}
		
		mkl_sparse_destroy(K);
		mkl_sparse_destroy(M);

		delete[] res;
	}
	else if (EIGSLV->SolverType == "APK") {
		// spectra eigen solver(arpack)
		int nev, ncv;
		nev = n0;			// Number of eigenvalues requested, nev must satisfy 1 <= nev <= n - 1
		ncv = 2 * nev;		// Parameter that controls the convergence speed of the algorithm,
		// ncv must satisfy nev < ncv <= n
		double sigma = 1.2345; // The value of the shift.

		// define sparse matrix K, M
		SpMat K, M;
		Matrix evecs;
		Vector evals;

		// CSR to SpMat
		int NNZ1 = iA[n] - iA[0];
		int NNZ2 = iB[n] - iB[0];
		K = csr2SpMat(A, iA, jA, n, NNZ1);
		M = csr2SpMat(B, iB, jB, n, NNZ2);

		// Eigen solver only uses the lower triangle
		OpType op(K, M);
		BOpType Bop(M);
		SymGEigsShiftSolver<OpType, BOpType, GEigsMode::ShiftInvert> eigs(op, Bop, nev, ncv, sigma);

		// begin solve 
		eigs.init();
		int maxit = 100;	// maxit = 100 to reduce running time for failed cases
		Eigen::Index nconv = eigs.compute(SortRule::LargestMagn, maxit);
		Eigen::Index niter = eigs.num_iterations();
		Eigen::Index nops = eigs.num_operations();

		if (eigs.info() != CompInfo::Successful) {
			printMsg.error("MathSolver::eigenSolve(): spectra eigen solver error ");
		}
		else {
			evals = eigs.eigenvalues();
			evecs = eigs.eigenvectors();
		}

		//eigenvalues is descending
		int j;
		for (int i = 0; i < nev; i++) {
			j = nev - i - 1;
			eigVal[j] = evals(i);
			memcpy(&eigVec[j * n], &evecs(i * n), n * sizeof(double));
		}
		m = n0;
	}
	else {
		printMsg.error("MathSolver::eigenSolve() : SolverType error ");
	}

	// print modal information
	std::cout << "Number of eigenvalues found: " << m << "\n";
	std::cout << " order    eigenvalues     frequency(Hz)" << "\n";
	logfile << "Number of eigenvalues found: " << m << "\n";
	logfile << "order    eigenvalues     frequency(Hz)" << "\n";
	for (int i = 0; i < m; i++) {
		printf("%4d %16.6e %16.6e \n", i + 1, eigVal[i], sqrt(eigVal[i]) / 2.0 / PI);
		logfile << "" << i + 1 << "  " << eigVal[i] << "  " << sqrt(eigVal[i]) / 2.0 / PI << "\n";
	}
	std::cout << "\n";
	logfile << "\n";
}

MathSolver MS;