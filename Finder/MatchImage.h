#pragma once
#include <array>
#include <iostream>
#include <fstream>
#include "Matrix.h"
using namespace std;
class MatchImage : public Matrix
{
public:
	MatchImage(int width, int height);
	MatchImage(Matrix* MatchImageMatrix);
	~MatchImage();
	double NNS_score;
	struct Location_coords
	{
		int width;
		int height;
	};
private:
};