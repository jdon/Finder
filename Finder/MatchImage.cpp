#include "MatchImage.h"
using namespace std;

MatchImage::MatchImage(int width, int height): Matrix(width,height)
{
	//creates an empty matrix of the size defined
}

MatchImage::MatchImage(Matrix * MatchImageMatrix) : Matrix(MatchImageMatrix->getWidth(), MatchImageMatrix->getHeight())
{

}

MatchImage::~MatchImage()
{
}
