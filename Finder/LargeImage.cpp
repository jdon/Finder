#include "LargeImage.h"
using namespace std;

LargeImage::LargeImage(string Filename, int width, int height) : Matrix(width, height)
{
	//populates the largeImage
	fillFromFile(Filename);
}

double LargeImage::NormalisedCorrelation(Matrix * Template, int width, int height, Matrix* WallyMean, Matrix* wallySquared)
{

	//Gets block of the cluttered scene image using the current row and col value and the size of the wally image
	Matrix* CreatedMatrix = this->CreateMatrix(width, height, Template->getWidth(), Template->getHeight());

	Matrix* CreatedMatrixMean = *CreatedMatrix - CreatedMatrix->Mean();

	Matrix* Multiply = *WallyMean * *CreatedMatrixMean;
	double topline = Multiply->Sum();

	Matrix* CreatedSquared = (*CreatedMatrixMean * *CreatedMatrixMean);

	double WallySq = wallySquared->Sum();
	double blockSq = CreatedSquared->Sum();

	double bottomline = sqrt(WallySq * blockSq);

	//Calculate NNS score
	double score = (topline / bottomline);

	delete Multiply;
	delete CreatedMatrix;
	delete CreatedMatrixMean;
	delete CreatedSquared;
	return score;
}

long LargeImage::SumOfSquaredDiff(Matrix * WallyMatrix, int width, int height)
{
	Matrix* CreatedMatrix = this->CreateMatrix(width, height, WallyMatrix->getWidth(), WallyMatrix->getHeight());
	Matrix* DifferenceMatrix = *(WallyMatrix)-*(CreatedMatrix);
	long score = 0;
	for (int i = 0; i < DifferenceMatrix->getWidth(); i++)
	{
		for (int j = 0; j < DifferenceMatrix->getHeight(); j++)
		{
			double diff = *(DifferenceMatrix->get(i, j));
			//cout << diff << " " << diff*diff << " ";
			long SquareDiff = diff * diff;
			score += SquareDiff;
		}
	}
	delete CreatedMatrix;
	delete DifferenceMatrix;
	return score;
}

