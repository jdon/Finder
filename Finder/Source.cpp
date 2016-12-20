#include "Matrix.h"
#include "LargeImage.h"
#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include <string>
using namespace std;

void DrawSquare(int WidthToStart, int HeightToStart, Matrix* DawOn, Matrix* ToDraw);
struct MatrixScore;
bool compareByScore(const MatrixScore a, const MatrixScore b);
bool compareByScoreNc(const MatrixScore a, const MatrixScore b);
vector<MatrixScore>* ScoreMatrix(LargeImage* BackGroundMatrix, Matrix* WallyMatrix, int method);
int getNum(string numberName, int defaultNum);
string getTextFile(string toAsk, string defaultFileName);
bool getBool(string toAask);
void Menu(vector<MatrixScore>* ncScores, vector<MatrixScore>* sdiffScores, string BackgroundTextFile, int BackgroundWidth, int BackgroundHeight, Matrix* WallyMatrix);

int main() 
{
	// Get input from user for the background
	string BackgroundTextFile = getTextFile("Background text file", "Cluttered_scene.txt");
	int BackgroundWidth = getNum("Width", 1024);
	int BackgroundHeight = getNum("Height", 768);
	LargeImage* Background = new LargeImage(BackgroundTextFile, BackgroundWidth, BackgroundHeight);

	//Get input from user for the wally template
	string WallyTextFile = getTextFile("Wally text file", "Wally_grey.txt");
	int WallyWidth = getNum("Width", 36);
	int WallyHeight = getNum("Height", 49);
	Matrix* WallyMatrix = new Matrix(WallyWidth, WallyHeight);
	WallyMatrix->fillFromFile(WallyTextFile);
	Background->WritePGM("redduded.pgm", 255);

	cout << "Computing scores for each image (Takes about 45 seconds)" << endl;
	vector<MatrixScore>* ncScores = ScoreMatrix(Background, WallyMatrix, 1);
	cout << "Normalised correlation completed" << endl;
	vector<MatrixScore>* sdiffScores = ScoreMatrix(Background, WallyMatrix, 2);
	cout << "Sum of squared differences completed" << endl;

	//Ask user how they want to compare the two inputs
	Menu(ncScores, sdiffScores, BackgroundTextFile, BackgroundWidth, BackgroundHeight,WallyMatrix);
	delete sdiffScores;
	delete ncScores;
	delete WallyMatrix;
	delete Background;
	system("pause");
	return 0;
}

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

struct MatrixScore
{
	int MatrixStartHeight;
	int MatrixStartWidth;
	int MatrixHeight;
	int MatrixWidth;
	int Method;
	double score;
};

bool compareByScore(const MatrixScore a, const MatrixScore b)
{
	return a.score < b.score;
}

bool compareByScoreNc(const MatrixScore a, const MatrixScore b)
{
	return a.score > b.score;
}

vector<MatrixScore>* ScoreMatrix(LargeImage* BackGroundMatrix, Matrix* WallyMatrix, int method) // returns a vector sorted by best result
{
	//create vector to save the scores and resize to fit all scores
	vector<MatrixScore>* scoreList = new vector<MatrixScore>;
	int amount = ((BackGroundMatrix->getWidth() - WallyMatrix->getWidth()) * (BackGroundMatrix->getHeight() - WallyMatrix->getHeight()));
	scoreList->resize(amount);

	//precompute wally values for normalised correlation, saves time as its not done every loop
	Matrix* WallyMean = *WallyMatrix - WallyMatrix->Mean();
	Matrix* wallySquared = (*WallyMean * *WallyMean);

	int count = 0;
	switch (method)
	{
	case 1:
		for (int i = 0; i < (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()); i++)
		{
			for (int j = 0; j < (BackGroundMatrix->getHeight() - WallyMatrix->getHeight()); j++)
			{
				double score = BackGroundMatrix->NormalisedCorrelation(WallyMatrix, i, j, WallyMean, wallySquared);
				MatrixScore Mscore;
				Mscore.MatrixStartHeight = j;
				Mscore.MatrixStartWidth = i;
				Mscore.MatrixHeight = WallyMatrix->getHeight();
				Mscore.MatrixWidth = WallyMatrix->getWidth();
				Mscore.Method = method;
				Mscore.score = score;
				scoreList->at(count) = Mscore;
				count++;
			}
		}
		break;
	case 2:
		//sum of squared diff
		for (int i = 0; i < (BackGroundMatrix->getWidth() - WallyMatrix->getWidth()); i++)
		{
			for (int j = 0; j < (BackGroundMatrix->getHeight() - WallyMatrix->getHeight()); j++)
			{
				double score = score = BackGroundMatrix->SumOfSquaredDiff(WallyMatrix, i, j);
				MatrixScore Mscore;
				Mscore.MatrixStartHeight = j;
				Mscore.MatrixStartWidth = i;
				Mscore.MatrixHeight = WallyMatrix->getHeight();
				Mscore.MatrixWidth = WallyMatrix->getWidth();
				Mscore.Method = method;
				Mscore.score = (score / 100);
				scoreList->at(count) = Mscore;
				count++;
			}
		}
		break;
	}
	delete WallyMean;
	delete wallySquared;
	if (method == 1)
	{
		sort(scoreList->begin(), scoreList->end(), compareByScoreNc);
	}
	else
	{
		sort(scoreList->begin(), scoreList->end(), compareByScore);
	}
	return scoreList;
}

int getNum(string numberName, int defaultNum)
{
	int num = 0;
	do
	{
		cout << numberName << " (" << defaultNum << ") ";
		string input;
		getline(cin, input);
		if (input.empty())
		{
			return defaultNum;
		}
		try
		{
			num = stoi(input);
		}
		catch (invalid_argument e)
		{
			cout << "Please enter a postive integer" << endl;
		}
		if (num > 0)
		{
			return num;
		}
	} while (num == 0 || num < 0);
}

string getTextFile(string toAsk, string defaultFileName)
{
	bool fileBad = true;
	do
	{
		string input_Scene;
		cout << toAsk << " (" << defaultFileName << " )";
		getline(cin, input_Scene);
		if (input_Scene.empty())
		{
			input_Scene = defaultFileName;
		}
		ifstream file(input_Scene);
		if (file.good())
		{
			if (input_Scene.substr(input_Scene.find_last_of(".") + 1) == "txt")
			{
				//file is a text file
				fileBad = false;
				return input_Scene;
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

bool getBool(string toAask)
{
	string input;
	do
	{
		cout << toAask << " (Yes) ";
		getline(cin, input);
		if (input.empty())
		{
			return true;
		}
		if (input == "n" || input == "no" || input == "false")
		{
			return false;
		}
		if (input == "y" || input == "yes" || input == "true")
		{
			return true;
		}
	} while (!(input == "y" || input == "yes" || input == "true" || input == "n" || input == "no" || input == "false"));
}

string getString(string toAsk, string defaultString) 
{
	string input;
	cout << toAsk << "(" << defaultString << ")";
	getline(cin, input);
	if (input.empty()) 
	{
		return defaultString;
	}
	else 
	{
		return input;
	}
}

void Menu(vector<MatrixScore>* ncScores, vector<MatrixScore>* sdiffScores ,string BackgroundTextFile, int BackgroundWidth, int BackgroundHeight, Matrix* WallyMatrix)
{
	int amount;
	do
	{
		LargeImage* Tochange = new LargeImage(BackgroundTextFile, BackgroundWidth, BackgroundHeight); // make a new background to draw on each time
		cout << "How would you like to compare the two images" << endl;
		cout << "1) Normalised correlation" << endl;
		cout << "2) Sum of squared difference" << endl;
		switch (getNum("Enter your choice", 1))
		{
		case 1:
		{
			amount = getNum("How many matches would you like?", 10);
			for (int i = 0; i < amount; i++)
			{
				cout << i << ") " << "x: " << ncScores->at(i).MatrixStartWidth << " y: " << ncScores->at(i).MatrixStartHeight << " score:" << ncScores->at(i).score << endl;
			}
			bool drawOn = getBool("Do you want to draw the selected scores over the background?");
			if (drawOn)
			{
				for (int i = 0; i < amount; i++)
				{
					MatrixScore ms = ncScores->at(i);
					DrawSquare(ms.MatrixStartWidth, ms.MatrixStartHeight, Tochange, WallyMatrix);
				}
				Tochange->WritePGM(getString("Name of pgm file","output")+".pgm", 255);
			}
			break;
		}
		case 2:
		{
			amount = getNum("How many matches would you like?", 10);
			for (int i = 0; i < amount; i++)
			{
				cout << i << ") " << "x: " << sdiffScores->at(i).MatrixStartWidth << " y: " << sdiffScores->at(i).MatrixStartHeight << " score:" << sdiffScores->at(i).score << endl;
			}
			bool drawOn = getBool("Do you want to draw the selected scores over the background?");
			if (drawOn)
			{
				for (int i = 0; i < amount; i++)
				{
					MatrixScore ms = sdiffScores->at(i);
					cout << " writing... " << ms.MatrixStartWidth << " " << ms.MatrixStartHeight;
					DrawSquare(ms.MatrixStartWidth, ms.MatrixStartHeight, Tochange, WallyMatrix);
				}
				Tochange->WritePGM(getString("Name of pgm file", "output") + ".pgm", 255);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	} while (true);
}