#pragma once
#include <array>;
#include <iostream>;
#include <fstream>;
using namespace std;
class Matrix
{
public:
	Matrix(int width, int height);
	~Matrix();
	double* get(int width, int height);
	int getWidth();
	int getHeight();
	void set(int width, int height, double x);
	void print();
	long CreateScore(Matrix* input);
	Matrix* Matrix::CreateMatrix(int startWidth, int startHeight, int MatrixWidth, int MatrixHeight);
	void fillFromFile(string filename);
	void WritePGM(string filename, int Q);
	Matrix* operator-(Matrix& InputedMatrix);
	Matrix* operator-(double InputNum);
	Matrix* operator*(Matrix& InputedMatrix);
	Matrix* operator=(Matrix& InputedMatrix);
	void reduce();
	double Mean();
	long Sum();

private:
	int width;
	int height;
	double** arr;
};