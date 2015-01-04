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


Mat findDFT(Mat image, Mat& magImage, Mat& phaseImage, int padding=0, int doLog=1)
{
	Mat padded = image.clone();

	if (padding)
		padImage(image, padded);

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
	fftShift(magImage, magImage);

	phase(planes[0], planes[1], phaseImage);
	phaseImage *= 180 / CV_PI ;
	phaseImage.convertTo(phaseImage, CV_8U);

	return dftImage;
}



void Conv2ByFFT(const Mat& f,const Mat& g,Mat& result)
{
result.create(abs(f.rows-g.rows)+1,abs(f.cols-g.cols)+1,f.type());

//pad the images and get optimal FFT size
Size dftSize;
dftSize.width = getOptimalDFTSize(f.cols + g.cols - 1);
dftSize.height = getOptimalDFTSize(f.rows + g.cols - 1);

Mat tmpF(dftSize,f.type(),Scalar::all(0));
Mat tmpG(dftSize,g.type(),Scalar::all(0));

Mat roiF(tmpF, Rect(0,0,f.cols,f.rows));
f.copyTo(roiF);
Mat roiG(tmpG, Rect(0,0,g.cols,g.rows));
g.copyTo(roiG);

//perform Fourier Transform
dft(tmpF,tmpF,0,f.rows);
dft(tmpG,tmpG,0,g.rows);

//perform per-element multiplication of two Fourier spectrums
mulSpectrums(tmpF,tmpG,tmpF,0);

//perform inverse Fourier Transform
dft(tmpF,tmpF,DFT_INVERSE+DFT_SCALE,result.rows);

tmpF(Rect(0,0,result.cols,result.rows)).copyTo(result);
}



void wiener(char *path, double SNR=5)
{
	Mat image = imread(path, 0);

	if (image.data == NULL)
	{
		cout << "invalid image";
		return ;
	}

	image.convertTo(image, CV_32F);
	image /= 255;

	Mat original = image.clone();
	Mat noise(image.rows, image.cols, CV_32F);
	Mat tempMean, meanImage, stdNoise, stdImage;

	meanStdDev(image, meanImage, stdImage);
	randn(noise, 0, .05);
	add(image, noise, image);

	subtract(image, meanImage, image);
	meanStdDev(image, tempMean, stdImage);
	stdNoise = (stdImage  / sqrt(SNR)) ;
	image += meanImage;

	Mat magImage, phaseImage, imageF;
	imageF = findDFT(image, magImage, phaseImage);

	Mat corrImage, temp1, temp2, temp3, filter;
	Mat noiseP = stdNoise * stdNoise * image.rows * image.cols;

	multiply(imageF, imageF, corrImage);
	add(corrImage, noiseP, temp1);
	divide(corrImage, temp1, temp2);
	multiply(temp2, imageF, temp2);

	//Mat kernel = Mat::ones( image.rows, image.cols, CV_32F )/ (float)(image.rows*image.cols);
	Mat kernel = getGaussianKernel(11,2,CV_32F);

	Mat magKernel, phaseKernel;
	Mat kernelF, temp21;// = findDFT(kernel, magKernel, phaseKernel);
	//mulSpectrums(temp2, kernelF, temp2, 0);
	//Conv2ByFFT(temp2,kernel,temp2);
	//cout << kernelF.rows << "\t" << kernelF.cols << endl ;



	dft(temp2, temp3, DFT_INVERSE|DFT_REAL_OUTPUT);
	normalize(temp3,  filter, 0, 1, CV_MINMAX);

	cout << filter.rows << "\t" << filter.cols << endl << image.rows << "\t" << image.cols << endl ;

	namedWindow("filter", 0);imshow("filter", filter);
	namedWindow("noise", 0);imshow("noise", image);
	namedWindow("original", 0);imshow("original", original);
	waitKey(0);
}
