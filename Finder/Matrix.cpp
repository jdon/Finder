#include "Matrix.h"
#include <array>;
#include <iostream>;
using namespace std;
Matrix::Matrix(int width, int height)
{
	this->width = width;
	this->height = height;
	arr = new double*[width];
	for (int i = 0; i < width; i++) 
	{
		arr[i] = new double[height];
		for (int j = 0; j < height; j++)
		{
			set(i, j, 0);
		}
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			set(i, j, 0);
		}
	}
}
Matrix::~Matrix()
{
	for (int i = 0; i < width; i++) 
	{
		delete[] arr[i];
	}
	delete[] arr;

}

double* Matrix::get(int width, int height)
{
	return &(arr[width][height]);
}

int Matrix::getWidth()
{
	return width;
}

int Matrix::getHeight()
{
	return height;
}

void Matrix::setWidth(int widthin)
{
	this -> width = widthin;
}

void Matrix::setHeight(int heightin)
{
	this->height = heightin;
}

void Matrix::set(int width, int height, double x)
{
	//std::cout << width << height << x << std::endl;
	arr[width][height] = x;
}

void Matrix::print()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			double d = *(get(i, j));
			std::cout << d << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Matrix::fillFromFile(string filename)
{
	int i = 0;

	ifstream file(filename);

	while (file.good())
	{
		if (i >= this->width * height)
			break;

		double d;
		file >> d;
		set(i%width, i / width, (int)d);
		i++;
	}
	file.close();
}

void Matrix::WritePGM(const string filename, int Q)
{
	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[width*height];

	// convert the integer values to unsigned char
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			double d = *(get(i, j));
			int num = i * height + j;
			image[(j*width) + i] = (unsigned char)d;
		}
	}

	myfile.open(filename, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		//cout << "Can't open file: " << filename << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << width << " " << height << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (width*height) * sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << "files" << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;
}
long Matrix::CreateScore(Matrix* input)
{
	long sum = 0;
	for (int i = 0; i < input->getWidth(); i++)
	{
		for (int j = 0; j < input->getHeight(); j++)
		{
			double diff = *(input->get(i, j));
			//cout << diff << " " << diff*diff << " ";
			long SquareDiff = diff * diff;
			sum += SquareDiff;
		}
	}
	//cout << endl << sum << endl;
	return sum;
}

Matrix* Matrix::CreateScoreMatrix(Matrix* Wally, Matrix* CompareMatrix)
{
	Matrix* DifferenMatrix = new Matrix(Wally->getWidth(), Wally->getHeight());
	for (int i = 0; i < Wally->getWidth(); i++)
	{
		for (int j = 0; j < Wally->getHeight(); j++)
		{
			double WallyNum = *(Wally->get(i, j));
			double ComapreNum = *(CompareMatrix->get(i, j));
			double diff = abs(WallyNum - ComapreNum);
			if (diff > 255)
			{
				//cout << "problem";
				cout << "i:" << i << "J:";
				cout << WallyNum << " " << ComapreNum << " " << diff << endl;
			}
			//cout << WallyNum << " " << ComapreNum << " " << diff <<endl;
			DifferenMatrix->set(i, j, diff);
		}
	}
	//cout << totalDif << endl;
	return DifferenMatrix;
}

Matrix* Matrix::CreateMatrix(int startr, int startc, Matrix* scene)
{
	Matrix* Imagee = new Matrix(36, 49);
	int rr = 0;
	int cc = 0;
	for (int i = startr; i < startr + 36; i++)
	{
		for (int j = startc; j < startc + 49; j++)
		{
			double d = *(scene->get(i, j));
			//cout << "width:" << j << " height:" << i << " " << d << " " << rr << " " << cc << endl;
			Imagee->set(i- startr, j- startc, d);;
			//cout << d << endl;
			cc++;
		}
		rr++;
		cc = 0;
	}
	return Imagee;
}
Matrix * Matrix::operator*(Matrix & InputedMatrix)
{
	Matrix* Imagee = new Matrix(this->getWidth(), this->getHeight());
	for (int i = 0; i < this->getWidth(); i++) 
	{
		for (int j = 0; j < this->getHeight(); j++) 
		{
			double CurrentMatrixValue = *(this->get(i, j));
			double InputedMatrixValue = *(InputedMatrix.get(i, j));
			double MultiValue = CurrentMatrixValue * InputedMatrixValue;
			Imagee->set(i, j, MultiValue);
		}
	}
	return Imagee;
}

Matrix* Matrix::operator-(Matrix& InputedMatrix)
{
	Matrix* DifferenMatrix = new Matrix(InputedMatrix.getWidth(), InputedMatrix.getHeight());
	for (int i = 0; i < InputedMatrix.getWidth(); i++)
	{
		for (int j = 0; j < InputedMatrix.getHeight(); j++)
		{
			double WallyNum = *(this->get(i, j));
			double ComapreNum = *(InputedMatrix.get(i, j));
			double diff = WallyNum - ComapreNum;
			if (diff > 255)
			{
				//cout << "problem";
				cout << "i:" << i << "J:";
				cout << WallyNum << " " << ComapreNum << " " << diff << endl;
			}
			//cout << WallyNum << " " << ComapreNum << " " << diff <<endl;
			DifferenMatrix->set(i, j, diff);
		}
	}
	//cout << totalDif << endl;
	return DifferenMatrix;
}
Matrix * Matrix::operator-(double InputNum)
{
	Matrix* Imagee = new Matrix(this->getWidth(), this->getHeight());
	for (int i = 0; i < this->getWidth(); i++)
	{
		for (int j = 0; j < this->getHeight(); j++)
		{
			double CurrentMatrixValue = *(this->get(i, j));
			double MinusValue = CurrentMatrixValue - InputNum;
			Imagee->set(i, j, MinusValue);
		}
	}
	return Imagee;
}
double Matrix::Sum()
{
	long sum = 0;
	for (int i = 0; i < this->getWidth(); i++)
	{
		for (int j = 0; j < this->getHeight(); j++)
		{
			double CurrentMatrixValue = *(this->get(i, j));
			sum += CurrentMatrixValue;
		}
	}
	return sum;
}
double Matrix::Mean()
{
	return (this->Sum() / (this->getWidth()*this->getHeight()));
}
long Matrix::CreateFuckingScore(Matrix* Wally, Matrix* CompareMatrix)
{
	long totalDif = 0;
	for (int i = 0; i < Wally->getWidth(); i++)
	{
		for (int j = 0; j < Wally->getHeight(); j++)
		{
			double WallyNum = *(Wally->get(i, j));
			double ComapreNum = *(CompareMatrix->get(i, j));
			double diff = abs(WallyNum - ComapreNum);
			//cout << WallyNum << " " << ComapreNum << " " << diff << " " << totalDif << endl;
			totalDif += diff;
		}
	}
	//cout << totalDif << endl;
	totalDif = totalDif / 1764;
	totalDif = totalDif * totalDif;
	return  totalDif;
}
