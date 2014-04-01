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

	//separate the planes of image
	Mat planes[3],dst[3] ;
	split(im1 ,planes);

	//set the angle
	float theta = CV_PI/6;

	//affine transformation matrix
	float data[] = {.4,.1,0,.1,-.1,0};
	Mat matrix = Mat(2,3,CV_32F,data).clone();

	//translation and scaling
	float data1[] = {2*1,0,100,0,2*1,100};
	Mat trans = Mat(2,3,CV_32F, data1).clone() ;

	//rotation by affine transformation
	float data2[] = {.4*cos(theta),.5*sin(theta),0,.3*(-sin(theta)),.2*cos(theta),1};
	Mat rot = Mat(2,3,CV_32F, data2).clone() ;

	//advanced rotation, around any point
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
