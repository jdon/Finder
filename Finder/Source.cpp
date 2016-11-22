#include "Matrix.h"
#include <iostream>
#include <vector>
#include <thread>
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
			Matrix* CreatedMatrix = BackGroundMatrix->CreateMatrix(i, j,WallyMatrix->getWidth(), WallyMatrix->getHeight());
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
			//createPercentage(cycles, (((BackGroundMatrix->getWidth() - WallyMatrix->getWidth())*(BackGroundMatrix->getHeight() - WallyMatrix->getHeight())) / 100));
			Matrix* CreatedMatrix = BackGroundMatrix->CreateMatrix(i, j, WallyMatrix->getWidth(), WallyMatrix->getHeight());
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
			//if (score > 0.203)
			//{
			//	lowest = score;
			//	cout << "lowest:" << score << " I:" << i << " J:" << j << endl;
			//	DrawSquare(i, j, ToChange, WallyMatrix);
			//}
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

double NormalisedCorrelation(Matrix* BackGroundMatrix, Matrix* WallyMatrix,int width, int height)
{
	Matrix* CreatedMatrix = BackGroundMatrix->CreateMatrix(width, height, WallyMatrix->getWidth(), WallyMatrix->getHeight());
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

struct MatrixScore
{
	int MatrixWidth;
	int MatrixHeight;
	int MatrixStartHeight;
	int MatrixStartWidth;
	double score;
};
int total = 0;
void createThread(Matrix* BackGroundMatrix, Matrix* WallyMatrix,int threadnum)
{
	vector<double> Top;
	double lowest = 0.15;
	switch (threadnum)
	{
	case 0:
		//first quarter
		for (int i = 0; i < (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()) / 2; i++)
		{
			for (int j = 0; j < (BackGroundMatrix->getHeight() - WallyMatrix->getHeight() / 2); j++)
			{
				double score = NormalisedCorrelation(BackGroundMatrix, WallyMatrix,i, j);
				total++;
			}
		}
		break;
	case 1:
		//second quarter
		for (int i = 0; i < (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()) / 2; i++)
		{
			for (int j = (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()) / 2; j < BackGroundMatrix->getHeight() - WallyMatrix->getHeight() ; j++)
			{
				double score = NormalisedCorrelation(BackGroundMatrix, WallyMatrix, i, j);
				total++;
			}
		}
	case 2:
		//first quarter
		for (int i = (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()) / 2; i < BackGroundMatrix->getWidth() - WallyMatrix->getWidth(); i++)
		{
			for (int j = 0; j < (BackGroundMatrix->getHeight() - WallyMatrix->getHeight() / 2); j++)
			{
				double score = NormalisedCorrelation(BackGroundMatrix, WallyMatrix, i, j);
				total++;
			}
		}
		break;
	case 3:
		//second quarter
		for (int i = (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()) / 2; i < BackGroundMatrix->getWidth() - WallyMatrix->getWidth(); i++)
		{
			for (int j = (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()) / 2; j < BackGroundMatrix->getHeight() - WallyMatrix->getHeight(); j++)
			{
				double score = NormalisedCorrelation(BackGroundMatrix, WallyMatrix, i, j);
				total++;
			}
		}
	default:
		break;
	}
}

//Matrix* ReduceMatrixSize(Matrix* MatrixToReduce) 
//{
//	for (int i = 0; i < MatrixToReduce->getWidth(); i++) 
//	{
//		for (int j = 0; j < MatrixToReduce->getHeight(); j++)
//		{
//
//		}
//	}
//}

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
	//thread t[4];
	//for (int i = 0; i < 4; i++) 
	//{
	//	t[i] = thread(createThread,BackGroundMatrix, WallyMatrix, i);
	//}
	//while (total <((BackGroundMatrix->getWidth() - WallyMatrix->getWidth())*(BackGroundMatrix->getHeight() - WallyMatrix->getHeight())))
	//{
	//	//cout << total << endl;
	//}
	//for (int i = 0; i < 4; i++) {
	//	cout << "thread " << i << "finished";
	//	t[i].join();
	//}
	NormalisedCorrelation(BackGroundMatrix, WallyMatrix,ToChange);
	ToChange->WritePGM("drawspuare.pgm", 255);
	delete WallyMatrix;
	delete BackGroundMatrix;
	//delete ToChange;
	return 0;
}