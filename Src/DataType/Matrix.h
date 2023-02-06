//Author		: Xiao Tuzi
//Date(Create)	: 01/03/2023
//Description	: this class is Matrix class


#pragma once
#include "Print/PrintMsg.h"
#include <iostream>
#include<complex>

template<typename T>
class Matrix {
private:
	int _row = 0;
	int _col = 0;
	T* _data = nullptr;
public:

	//no-parameter constructor
	Matrix();

	//parameter constructor
	Matrix(int row, int col);

	//parameter constructor(initial value)
	Matrix(int row, int col, T src);

	// copy constructor
	Matrix(const Matrix<T>& M);

	// deconstructor
	~Matrix();

	// get row
	inline int rows();

	// get col
	inline int cols();

	// get data(first address)
	inline T* datas();

	// resize function
	inline void setSize(int row, int col);
	inline void setSize(int row, int col, T src);

	// destroy function
	inline void destroy();

	// show function
	inline void show();

	// operator =
	Matrix& operator = (T src);

	// operator =
	Matrix& operator = (const Matrix<T>& M);

	// operator +
	Matrix  operator + (const Matrix<T>& M);

	// operation -
	Matrix  operator - (const Matrix<T>& M);

	// operation *
	Matrix  operator * (const T& alpha);

	// get A(i, j)
	T& operator ()(int row, int col);

	// get A[i] by index
	T& operator [](int index);

	// transpose()
	Matrix<T> transpose();
};



// definition of Matrix class

//no-parameter constructor
template<typename T>
Matrix<T>::Matrix() {
	_row = 0;
	_col = 0;
	_data = nullptr;
};

//parameter constructor
template<typename T>
Matrix<T>::Matrix(int row, int col) {
	if (row > 0 && col > 0) {
		_row = row;
		_col = col;
		_data = new T[row * col]();
	}
	else {
		printMsg.error("Matrix::Matrix(): row/col must be positive");
	}
};

//parameter constructor(initial value)
template<typename T>
Matrix<T>::Matrix(int row, int col, T src) {
	if (row > 0 && col > 0) {
		_row = row;
		_col = col;
		_data = new T[row * col]();
		for (int i = 0; i < _row * _col; i++) {
			_data[i] = src;
		}
	}
	else {
		printMsg.error("Matrix::Matrix(): row/col must be positive");
	}
};

// copy constructor
template<typename T>
Matrix<T>::Matrix(const Matrix<T>& M) {
	if (M._row > 0 && M._col > 0) {
		_row = M._row;
		_col = M._col;
		_data = new T[_row * _col]();
		memcpy(_data, M._data, _row * _col * sizeof(T));
	}
	else {
		printMsg.error("Matrix::Matrix(): row/col must be positive");
	}
};

// deconstructor
template<typename T>
Matrix<T>::~Matrix() {
	if (_data) {
		delete[] _data;
		_data = nullptr;
	}
};

// rows() function
template<typename T>
inline int Matrix<T>::rows() {
	return _row;
};

// cols() function
template<typename T>
inline int Matrix<T>::cols() {
	return _col;
};

// datas() function (first address)
template<typename T>
inline T* Matrix<T>::datas() {
	return _data;
};

// setSize() function
template<typename T>
inline void Matrix<T>::setSize(int row, int col) {
	if (row > 0 && col > 0) {
		if (_data == nullptr) {
			_row = row;
			_col = col;
			_data = new T[_row * _col]();
		}
		else {
			printMsg.error("Matrix::setSize(): original matrix must be empty");
			exit(-1);
		}
	}
	else {
		printMsg.error("Matrix::setSize(): row/col must be positive");
		exit(-1);
	}
};

// setSize() function
template<typename T>
inline void Matrix<T>::setSize(int row, int col, T src) {
	if (row > 0 && col > 0) {
		if (_data == nullptr) {
			_row = row;
			_col = col;
			_data = new T[_row * _col]();
			for (int i = 0; i < _row * _col; i++){
				_data[i] = src;
			}
		}
		else {
			printMsg.error("Matrix::setSize(): original matrix must be empty");
			exit(-1);
		}
	}
	else {
		printMsg.error("Matrix::setSize(): row/col must be positive");
		exit(-1);
	}
};

// destroy() function
template<typename T>
inline void Matrix<T>::destroy() {
	this->~Matrix();
};

// show() function
template<typename T>
inline void Matrix<T>::show() {
	if (_row == 0 || _col == 0 || _data == nullptr) {
		printMsg.error("Matrix::show(): this matrix is empty");
	}
	else {
		for (int i = 0; i < _row * _col; i++) {
			std::cout << _data[i] << "  ";
			if ((i + 1) % _col == 0) { std::cout << "\n"; }
		}
		std::cout << "\n";
	}
};

// operator =
template<typename T>
Matrix<T>& Matrix<T>::operator = (T src) {
	for (int i = 0; i < _row * _col; i++) {
		_data[i] = src;
	}
	return *this;
};

// operator =
template<typename T>
Matrix<T>& Matrix<T>::operator = (const Matrix<T>& M) {
	if (_row == M._row && _col == M._col) {
		memcpy(_data, M._data, _row * _col * sizeof(T));
		return *this;
	}
	else {
		printMsg.error("Matrix::operator=: row/col must be equal");
	}
};

// operator +, A + B
template<typename T>
Matrix<T>  Matrix<T>::operator + (const Matrix<T>& M) {
	if (_row == M._row && _col == M._col) {
		Matrix result(M._row, M._col);
		for (int i = 0; i < _row * _col; i++) {
			result._data[i] = _data[i] + M._data[i];
		}
		return result;
	}
	else {
		printMsg.error("Matrix::operator+: row/col must be equal");
	}
};

// operation -, A - B
template<typename T>
Matrix<T>  Matrix<T>::operator - (const Matrix<T>& M) {
	if (_row == M._row && _col == M._col) {
		Matrix result(M._row, M._col);
		for (int i = 0; i < _row * _col; i++) {
			result._data[i] = _data[i] - M._data[i];
		}
		return result;
	}
	else {
		printMsg.error("operator -: row/col must be equal");
	}
};

// operation *, A * alpha
template<typename T>
Matrix<T>  Matrix<T>::operator * (const T& alpha) {
		Matrix result(_row, _col);
		for (int i = 0; i < _row * _col; i++) {
			result._data[i] = _data[i] * alpha;
		}
		return result;
};

// get A(i, j)
template<typename T>
T& Matrix<T>::operator ()(int i, int j) {
	if (i < _row && j < _col && i >= 0 && j >= 0) {
		return _data[j + i * _col];
	}
	else {
		printMsg.error("Matrix::operator(i, j): i/j out of range");
	}
};

// get A[i]
template<typename T>
T& Matrix<T>::operator [](int index) {
	if (index < _row * _col && index >= 0) {
		return _data[index];
	}
	else {
		printMsg.error("Matrix::operator[i]: index is out of range");
	}
};

// transpose function
template<typename T>
Matrix<T> Matrix<T>::transpose() {
	Matrix<T> trans(_col, _row);
	for (int i = 0; i < _row; i++) {
		for (int j = 0; j < _col; j++) {
			trans(j, i) = _data[j + i * _col];
		}
	}
	return trans;
};

using iMatrix = Matrix<int>;
using fMatrix = Matrix<float>;
using dMatrix = Matrix<double>;
using cMatrix = Matrix<std::complex<float>>;
using zMatrix = Matrix<std::complex<double>>;