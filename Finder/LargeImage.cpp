#include "LargeImage.h"
using namespace std;

LargeImage::LargeImage(int width, int height, string Filename) : Matrix(width, height)
{
	//populates the largeImage
	fillFromFile(Filename);
}

double LargeImage::NormalisedCorrelation(Matrix * WallyMatrix, int width, int height)
{
	Matrix* CreatedMatrix = this->CreateMatrix(width, height, WallyMatrix->getWidth(), WallyMatrix->getHeight());
	Matrix* WallyMean = (*(WallyMatrix)-WallyMatrix->Mean());
	Matrix* CreatedMatrixMean = (*(CreatedMatrix)-CreatedMatrix->Mean());
	Matrix* Multiply = *(WallyMean) * *(CreatedMatrixMean);
	double TopLine = Multiply->Sum();

	Matrix* WallyBot = *(WallyMean) * *(WallyMean);
	double WallyBotValue = WallyBot->Sum();
	Matrix* CreatedMatrixBot = *(CreatedMatrix) * *(CreatedMatrix);
	double CreatedMatrixValue = CreatedMatrixBot->Sum();
	double bottom = WallyBotValue*CreatedMatrixValue;
	double sqrtbottom = sqrt(bottom);

	//cout << "topLine:" << TopLine << "wallyBotValue: " << WallyBotValue << " CreatedMatrixValue:" << CreatedMatrixValue << " bottom:" << bottom << "sqrtbottom" << sqrtbottom << endl;

	double score = TopLine / sqrtbottom;
	//cout << "score:" << score;
	delete CreatedMatrix;
	delete WallyMean;
	delete CreatedMatrixMean;
	delete Multiply;
	delete WallyBot;
	delete CreatedMatrixBot;
	return score;
}

double LargeImage::SumOfSquaredDiff(Matrix * WallyMatrix, int width, int height)
{
	Matrix* CreatedMatrix = this->CreateMatrix(width, height, WallyMatrix->getWidth(), WallyMatrix->getHeight());
	Matrix* DifferenceMatrix = *(WallyMatrix)-*(CreatedMatrix);
	long score = WallyMatrix->CreateScore(DifferenceMatrix); // sum of squard differences
	delete CreatedMatrix;
	delete DifferenceMatrix;
	return score;
}

double LargeImage::SquaredSumDiff(Matrix * WallyMatrix, int width, int height)
{
	Matrix* CreatedMatrix = this->CreateMatrix(width, height, WallyMatrix->getWidth(), WallyMatrix->getHeight());
	Matrix* DifferenceMatrix = *(WallyMatrix)-*(CreatedMatrix);
	long score = WallyMatrix->CreateFuckingScore(WallyMatrix, CreatedMatrix); // squared sum of differences
	delete CreatedMatrix;
	delete DifferenceMatrix;
	return score;
}

//double LargeImage::Score(int type, bool threading, Matrix* WallyMatrix)
//{
//	int lowest = 1010000000;
//	for (int i = 0; i < this->getWidth() - WallyMatrix->getWidth(); i++)
//	{
//		for (int j = 0; j < this->getHeight() - WallyMatrix->getHeight(); j++)
//		{
//			Matrix* CreatedMatrix = this->CreateMatrix(162, 144, WallyMatrix->getWidth(), WallyMatrix->getHeight());
//			Matrix* DifferenceMatrix = *(WallyMatrix)-*(CreatedMatrix);
//			long score = WallyMatrix->CreateScore(DifferenceMatrix); // sum of squard differences
//			long score  = WallyMatrix->CreateFuckingScore(WallyMatrix, CreatedMatrix); // squared sum of differences
//			cout << "lowest:" << score << " I:" << i << " J:" << j << endl;
//			if (score < lowest)
//			{
//				lowest = score;
//				cout << "lowest:" << score << " I:" << i << " J:" << j << endl;
//				DrawSquare(i, j, ToChange, WallyMatrix);
//			}
//			delete CreatedMatrix;
//			delete DifferenceMatrix;
//		}
//	}
//}
