#pragma once
#ifndef DERIVED_H
#define DERIVED_H
#include "Matrix.h"
using namespace std;
class LargeImage : public Matrix 
{
public:
	LargeImage(int width, int height, string Filename);
	double LargeImage::NormalisedCorrelation(Matrix* WallyMatrix, int width, int height);
	double LargeImage::SumOfSquaredDiff(Matrix* WallyMatrix, int width, int height);
	double LargeImage::SquaredSumDiff(Matrix* WallyMatrix, int width, int height);
};
#endif