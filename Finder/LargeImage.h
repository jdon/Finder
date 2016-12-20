#pragma once
#ifndef DERIVED_H
#define DERIVED_H
#include "Matrix.h"
using namespace std;
class LargeImage : public Matrix 
{
public:
	LargeImage(string Filename, int width, int height);
	double LargeImage::NormalisedCorrelation(Matrix* WallyMatrix, int width, int height, Matrix* WallyMean, Matrix* wallySquared);
	long LargeImage::SumOfSquaredDiff(Matrix* WallyMatrix, int width, int height);
};
#endif