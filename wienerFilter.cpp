#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

void padImage(Mat image, Mat& padImage)
{
	//pad image with zeros.
	int m = getOptimalDFTSize( image.rows );
	int n = getOptimalDFTSize( image.cols );
	copyMakeBorder(image, padImage, m-image.rows, 0, n-image.cols, 0, BORDER_REPLICATE);
}


void fftShift(Mat magImage, Mat& shiftImage)
{
	magImage.copyTo(shiftImage);

	int cx = shiftImage.cols/2;
	int cy = shiftImage.rows/2;

	Mat q0(shiftImage, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(shiftImage, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(shiftImage, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(shiftImage, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);


}


Mat findDFT(Mat padded, Mat& magImage, Mat& phaseImage, int doLog)
{
	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat dftImage;

	merge(planes, 2, dftImage);
	dft(dftImage, dftImage);
	split(dftImage, planes);

	magnitude(planes[0], planes[1], magImage);

	//switch to logarithmic scale
	if( doLog )
	{
		magImage += Scalar::all(1);
		log(magImage, magImage);
	}

	// crop the spectrum, if it has an odd number of rows or columns
	magImage = magImage(Rect(0, 0, magImage.cols & -2, magImage.rows & -2));
	normalize(magImage, magImage, 0, 1, CV_MINMAX);

	phase(planes[0], planes[1], phaseImage);
	phaseImage *= 180 / CV_PI ;
	phaseImage.convertTo(phaseImage, CV_8U);

	return dftImage;


}


Mat filter(Mat image)
{
	/*Mat image = imread(path, 0);
	//Mat image = getGaussianKernel(57, .0001, CV_64F);

	//double der_x[] = {-3, 0, 3, -10, 0, 10, -3, 0, 3} ;
	//Mat image = Mat(3, 3, CV_32F, der_x).clone();

	if ( image.data == NULL)
		cout << "invalid image";

	image.convertTo(image, CV_32F);
*/

	Mat padded = image.clone();
	padImage(image, padded);

	Mat magImage, phaseImage, dftImage;
	dftImage = findDFT(padded, magImage, phaseImage, 0);

	return dftImage;
/*
	Mat shiftImage = magImage.clone();
	fftShift(magImage, shiftImage);
*/
	//cout << magImage.rows << "\t" << magImage.cols << endl ;


	//namedWindow("dft",0);imshow("dft", shiftImage);
	//waitKey(0);




}


void correlation(char *path)
{
	Mat image = imread(path, 0);

	if (image.data == NULL)
	{
		cout << " invalid image";
		return ;
	}

	image.convertTo(image, CV_32F);
	image /= 255;
	Mat original = image.clone();

	//Mat noise(image.rows, image.cols, CV_32F);
	//randn(noise, 0, .1);

	Mat meanNoise, meanImage, std_dev, std;
	//meanStdDev(noise, meanNoise, std_dev);
	meanStdDev(image, meanImage, std);
	subtract(image, meanImage, image);
	meanStdDev(image, meanImage, std);std_dev = std / 2 ;

	Mat imageF = filter(image), noiseF = filter(noise);
	Mat corrImage, corrNoise, temp1, temp2, temp3;

	multiply(imageF, imageF, corrImage);
	multiply(noiseF, noiseF, corrNoise);

	add(corrImage, std_dev * std_dev * image.rows * image.cols, temp1);
	divide(corrImage, temp1, temp2);
	multiply(temp2, imageF, temp2);

	dft(temp2,  temp3, DFT_INVERSE|DFT_REAL_OUTPUT);
	normalize( temp3,  temp3, 0, 1, CV_MINMAX);

	cout << std / std_dev << endl;
	namedWindow("filter",0);imshow("filter",temp3 );
	//namedWindow("noise",0);imshow("noise",noise );
	namedWindow("original",0);imshow("original",original );
	waitKey(0);
}
