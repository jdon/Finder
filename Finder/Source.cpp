#include "Matrix.h"
#include <iostream>
#include <vector>
using namespace std;


void DrawSquare(int WidthToStart, int HeightToStart, Matrix* DawOn, Matrix* ToDraw)
{
	for (int i = WidthToStart; i < WidthToStart + ToDraw->getWidth(); i++)
	{
		for (int j = HeightToStart; j < HeightToStart + ToDraw->getHeight(); j++)
		{
			//draw square
			if (i == WidthToStart || i == (WidthToStart + ToDraw->getWidth()) - 1 || j == HeightToStart || j == (HeightToStart + ToDraw->getHeight()) - 1)
			{
				DawOn->set(i, j, 0);
			}
			//DawOn->set(i, j, 0);
		}
	}
}

void ScoreMatrix(Matrix* BackGroundMatrix, Matrix* WallyMatrix, Matrix* ToChange) 
{
	int lowest = 11000000;
	for (int i = 0; i < BackGroundMatrix->getWidth() - WallyMatrix->getWidth(); i++)
	{
		for (int j = 0; j < BackGroundMatrix->getHeight() - WallyMatrix->getHeight(); j++)
		{
			Matrix* CreatedMatrix = WallyMatrix->CreateMatrix(i, j, BackGroundMatrix);
			Matrix* ScoreMatrix = *(WallyMatrix)-*(CreatedMatrix);
			long score = WallyMatrix->CreateScore(ScoreMatrix); // sum of squard differences
			//long score  = WallyMatrix->CreateFuckingScore(WallyMatrix, CreatedMatrix); // squared sum of differences
			if (score < lowest)
			{
				//lowest = score;
				cout << "lowest:" << score << " I:" << i << " J:" << j << endl;
				DrawSquare(i, j, ToChange, WallyMatrix);
			}
			delete CreatedMatrix;
			delete ScoreMatrix;
		}
	}
}

void createPercentage(double cycles, double totalAmount) 
{
	//system("cls");
	int percent = cycles / totalAmount+1;
	cout << "[";
	for (int i = 0; i < percent; i++)
	{
		cout << "|";
	}
	cout << "]" << "-" << percent << endl;
}
void NormalisedCorrelation(Matrix* BackGroundMatrix, Matrix* WallyMatrix, Matrix* ToChange)
{
	double lowest = 0.12;
	double cycles = 0;
	Matrix* WallyMean = (*(WallyMatrix)-WallyMatrix->Mean());
	Matrix* WallyBot = *(WallyMean) * *(WallyMean);
	double WallyBotValue = WallyBot->Sum();
	for (int i = 0; i < BackGroundMatrix->getWidth() - WallyMatrix->getWidth(); i++)
	{
		for (int j = 0; j < BackGroundMatrix->getHeight() - WallyMatrix->getHeight(); j++)
		{
			createPercentage(cycles, (((BackGroundMatrix->getWidth() - WallyMatrix->getWidth())*(BackGroundMatrix->getHeight() - WallyMatrix->getHeight())) / 100));
			//int percent = cycles / (((BackGroundMatrix->getWidth() - WallyMatrix->getWidth())*(BackGroundMatrix->getHeight() - WallyMatrix->getHeight()))/100);
			//cout << percent << "% " << cycles << endl;
			Matrix* CreatedMatrix = WallyMatrix->CreateMatrix(i, j, BackGroundMatrix);
			/*Matrix* WallyMean = (*(WallyMatrix)-WallyMatrix->Mean());*/
			Matrix* CreatedMatrixMean = (*(CreatedMatrix)-CreatedMatrix->Mean());
			Matrix* Multiply = *(WallyMean) * *(CreatedMatrixMean);
			double TopLine = Multiply->Sum();

			//Matrix* WallyBot = *(WallyMean) * *(WallyMean);
			//double WallyBotValue = WallyBot->Sum();

			Matrix* CreatedMatrixBot = *(CreatedMatrix) * *(CreatedMatrix);
			double CreatedMatrixValue = CreatedMatrixBot->Sum();
			double bottom = WallyBotValue*CreatedMatrixValue;
			double sqrtbottom = sqrt(bottom);

			double score = TopLine / sqrtbottom;
			if (score > 0.203)
			{
				lowest = score;
				cout << "lowest:" << score << " I:" << i << " J:" << j << endl;
				DrawSquare(i, j, ToChange, WallyMatrix);
			}
			delete CreatedMatrix;
			/*delete WallyMean;*/
			delete CreatedMatrixMean;
			delete Multiply;
			/*delete WallyBot;*/
			delete CreatedMatrixBot;
			//delete ScoreMatrix;
			cycles++;
		}
	}
	delete WallyMean;
	delete WallyBot;
}

void NormalisedCorrelation(Matrix* BackGroundMatrix, Matrix* WallyMatrix)
{
	Matrix* CreatedMatrix = WallyMatrix->CreateMatrix(0, 0, BackGroundMatrix);
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

	cout << "topLine:" << TopLine << "wallyBotValue: " << WallyBotValue << " CreatedMatrixValue:" << CreatedMatrixValue << " bottom:" << bottom << "sqrtbottom" << sqrtbottom << endl;

	double score = TopLine / sqrtbottom;
	cout << "score:" << score;
	delete CreatedMatrix;
	delete WallyMean;
	delete CreatedMatrixMean;
	delete Multiply;
	delete WallyBot;
	delete CreatedMatrixBot;
}

vector<Matrix*> Top;

int main() 
{
	Matrix* WallyMatrix = new Matrix(36, 49);
	WallyMatrix->fillFromFile("Wally_grey.txt");

	Matrix* BackGroundMatrix = new Matrix(1024, 768);
	BackGroundMatrix->fillFromFile("Cluttered_scene.txt");

	Matrix* ToChange = new Matrix(1024, 768);
	ToChange->fillFromFile("Cluttered_scene.txt");

	//ScoreMatrix(BackGroundMatrix, WallyMatrix, ToChange);
	//ToChange->WritePGM("drawspuare.pgm", 255);

	Matrix* MulitPlyTest = *(WallyMatrix) * *(WallyMatrix);
	Matrix* WallyMinusMean = *(WallyMatrix)-WallyMatrix->Mean();
	NormalisedCorrelation(BackGroundMatrix, WallyMatrix,ToChange);
	ToChange->WritePGM("drawspuare.pgm", 255);
	delete WallyMatrix;
	delete BackGroundMatrix;
	delete ToChange;
	delete MulitPlyTest;
	delete WallyMinusMean;
	return 0;
}