#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

void transform()
{
	char* path = "/home/powel/codes/powel2/src1/dp1.png" ;
	Mat im1 = imread(path,3);

	int channels = im1.channels();
	int col = im1.cols;
	int row = im1.rows;

	Mat planes[3],dst[3] ;
	split(im1 ,planes);

	float theta = 0;
	float data[] = {.04,.8121,0,-0.5,.522,0};
	Mat matrix = Mat(2,3,CV_32F,data).clone();

	Mat rotate = getRotationMatrix2D( Point(col/2,row/2), theta, 0 );

	for(int i=0; i<channels; i++)
	{
		cout<< i<<endl ;
		warpAffine(planes[i], dst[i], matrix, Size(col,row));

	}
	Mat final;
	merge(dst,3,final);

	namedWindow("original", 0);
	imshow("original",im1);

	namedWindow("final",0);
	imshow("final",final);
	waitKey(0);
}
