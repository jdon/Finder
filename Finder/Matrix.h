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
	void setWidth(int rowIn);
	void setHeight(int colIn);
	void set(int width, int height, double x);
	void print();
	long CreateScore(Matrix* input);
	Matrix* CreateScoreMatrix(Matrix* Wally, Matrix* CompareMatrix);
	Matrix* CreateMatrix(int startr, int startc, Matrix* scene);
	long CreateFuckingScore(Matrix* Wally, Matrix* CompareMatrix);
	void fillFromFile(string filename);
	void WritePGM(string filename, int Q);
	Matrix* operator-(Matrix& InputedMatrix);
	Matrix* operator-(double InputNum);
	Matrix* operator*(Matrix& InputedMatrix);
	double Mean();
	double Sum();

private:
	int width;
	int height;
	double** arr;
};