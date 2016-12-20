#include "Matrix.h"
using namespace std;
Matrix::Matrix(int width, int height)
{
	this->width = width;
	this->height = height;
	arr = new double*[this->width];
	for (int i = 0; i < this->width; i++)
	{
		arr[i] = new double[this->height];
		for (int j = 0; j < this->height; j++)
		{
			set(i, j, 0);
		}
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < width; i++) 
	{
		delete[] this->arr[i];
	}
	delete[] this->arr;

}

double* Matrix::get(int width, int height)
{
		return &(this->arr[width][height]);
}

int Matrix::getWidth()
{
	return this->width;
}

int Matrix::getHeight()
{
	return this->height;
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

Matrix* Matrix::CreateMatrix(int startWidth, int startHeight,int MatrixWidth,int MatrixHeight)
{
	Matrix* Imagee = new Matrix(MatrixWidth, MatrixHeight);
	for (int i = startWidth; i < startWidth + MatrixWidth; i++)
	{
		for (int j = startHeight; j < startHeight + MatrixHeight; j++)
		{
			double d = 0;
			if (i < this->getWidth() && j < this->getHeight()) 
			{
				//safe to get the number or else set it to 0
				d = *(this->get(i, j));
			}
			//cout << "width:" << j << " height:" << i << " " << d << " " << rr << " " << cc << endl;
			Imagee->set(i- startWidth, j- startHeight, d);;
			//cout << d << endl;
		}
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

Matrix * Matrix::operator=(Matrix & InputedMatrix)
{
	//delete the old data in the matrix and set the new data
	for (int i = 0; i < width; i++)
	{
		delete[] this->arr[i];
	}
	delete[] this->arr;

	this->width = InputedMatrix.getWidth();
	this->height = InputedMatrix.getHeight();

	arr = new double*[this->width];

	for (int i = 0; i < this->width; i++)
	{
		arr[i] = new double[this->height];
		for (int j = 0; j < this->height; j++)
		{
			this->set(i, j, *(InputedMatrix.get(i,j)));
		}
	}
	return this;
}

void Matrix::reduce()
{
	int width = (int)round(this->width / 2);
	int height = (int)round((this->getHeight() / 2) + 0.5);
	Matrix* Reduced = new Matrix(width, height);
	int ired = 0;
	int jred = 0;
	for (int i = 0; i < this->getWidth(); i += 2)
	{
		for (int j = 0; j < this->getHeight(); j += 2)
		{
			Matrix* pixel = this->CreateMatrix(i, j, 2, 2);
			double mean = pixel->Mean();
			Reduced->set((int)(i / 2), (int)(j / 2), mean);
			delete pixel;
		}
	}
	*this = *Reduced;
	delete Reduced;
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
			//cout << diff << endl;
			DifferenMatrix->set(i, j, diff);
		}
	}
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

long Matrix::Sum()
{
	long sum = 0;
	for (int i = 0; i < this->getWidth(); i++)
	{
		for (int j = 0; j < this->getHeight(); j++)
		{
			int CurrentMatrixValue = *(this->get(i, j));
			sum = sum + CurrentMatrixValue;
		}
	}
	return sum;
}

double Matrix::Mean()
{
	return (this->Sum() / (this->getWidth()*this->getHeight()));
}