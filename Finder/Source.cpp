#include "Matrix.h"
#include "LargeImage.h"
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
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
void ScoreMatrix(LargeImage* BackGroundMatrix, Matrix* WallyMatrix, Matrix* ToChange) 
{
	int lowest = 1010000000;
	for (int i = 0; i < BackGroundMatrix->getWidth() - WallyMatrix->getWidth(); i++)
	{
		for (int j = 0; j < BackGroundMatrix->getHeight() - WallyMatrix->getHeight(); j++)
		{
			Matrix* CreatedMatrix = BackGroundMatrix->CreateMatrix(162, 144,WallyMatrix->getWidth(), WallyMatrix->getHeight());
			Matrix* DifferenceMatrix = *(WallyMatrix)-*(CreatedMatrix);
			long score = WallyMatrix->CreateScore(DifferenceMatrix); // sum of squard differences
			//long score  = WallyMatrix->CreateFuckingScore(WallyMatrix, CreatedMatrix); // squared sum of differences
			//cout << "lowest:" << score << " I:" << i << " J:" << j << endl;
			if (score < lowest)
			{
				lowest = score;
				cout << "lowest:" << score << " I:" << i << " J:" << j << endl;
				DrawSquare(i, j, ToChange, WallyMatrix);
			}
			delete CreatedMatrix;
			delete DifferenceMatrix;
		}
	}
}
struct MatrixScore
{
	int MatrixWidth;
	int MatrixHeight;
	int MatrixStartHeight;
	int MatrixStartWidth;
	double score;
};

int getNum(string numberName, int defaultNum) 
{
	int num = 0;
	do 
	{
		cout << numberName<< " (" << defaultNum  << ")" << endl;
		cin >> num;
		if (num == 0) cout << "kek" << endl;
		cin.clear();
		cin.ignore();
		if (num > 0) 
		{
			return num;
		}
	} while (num == 0 || num < 0);
}

Matrix* getTextFile(string fileName)
{
	bool fileBad = true;
	do
	{
		string input_Scene;
		cout << "Please select "<< fileName <<" text file" << endl;
		getline(cin, input_Scene);
		ifstream file(input_Scene);
		if (file.good())
		{
			if (input_Scene.substr(input_Scene.find_last_of(".") + 1) == "txt")
			{
				//file is a text file
				fileBad = false;
				int width = getNum("width", 10);
				int height = getNum("height", 10);
				Matrix* matrix = new Matrix(width, height);
				matrix->fillFromFile(input_Scene);
				return matrix;
			}
			else
			{
				cout << input_Scene << " is not a text file" << endl;
			}
		}
		else
		{
			cout << input_Scene << " does not exist" << endl;
		}
	} while (fileBad);
}

bool multithreading = false, reduceImage = false;

void Menu() 
{
	int amount;
	do 
	{
		cout << "How would you like to compare the two images" << endl;
		cout << "1) Sum of squared difference" << endl;
		cout << "2) Squared sum of differences" << endl;
		cout << "3) Normalised correlation" << endl;
		cout << "4) Enable multithreading" << endl;
		cout << "5) Reduce image size (Faster but less accurate)" << endl;
		switch (getNum("Enter your choice",10)) 
		{
		case 1:
			amount = getNum("How many matches would you like?",10);
			break;
		case 2:
			amount = getNum("How many matches would you like?", 10);
			break;
		case 3:
			amount = getNum("How many matches would you like?", 10);
			break;
		case 4:
			if (multithreading) 
			{
				cout << endl << "Multithreading has been disabled" << endl << endl;
				multithreading = false;
				break;
			}
			else 
			{
				cout << endl << "Multithreading has been enabled" << endl << endl;
				multithreading = true;
				break;
			}
		case 5:
			if (reduceImage)
			{
				cout << endl << "Reduce image has been disabled" << endl << endl;
				reduceImage = false;
				break;
			}
			else
			{
				cout << endl << "Reduce image has been enabled" << endl << endl;
				reduceImage = true;
				break;
			}
		default: break;
		}
	} while (true);
}
int main() 
{
	Menu();
	Matrix* Background =  getTextFile("background");
	Matrix* Wally = getTextFile("wally");
	system("pause");
	Matrix* WallyMatrix = new Matrix(36, 49);
	WallyMatrix->fillFromFile("Wally_grey.txt");
	Matrix* BackGroundMatrix = new Matrix(1024, 768);
	BackGroundMatrix->fillFromFile("Cluttered_scene.txt");
	Matrix* ToChange = new Matrix(1024, 768);
	ToChange->fillFromFile("Cluttered_scene.txt");
	//ScoreMatrix(BackGroundMatrix, WallyMatrix, ToChange);

	ToChange->WritePGM("text.pgm",255);


	//Matrix* Redu = ReduceMatrixSize(BackGroundMatrix);
	//Matrix* change = ReduceMatrixSize(BackGroundMatrix);
	//Matrix* WallyRedu = ReduceMatrixSize(WallyMatrix);
	delete WallyMatrix;
	delete BackGroundMatrix;
	return 0;
}