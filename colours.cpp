#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

void colours()
{
	char* path = "/home/powel/codes/powel2/src1/dp1.png" ;
	Mat image= imread(path,3);
	Mat planes[3];
	Mat im2 = image.clone();
	split(im2 ,planes);

	namedWindow("red" , 0);
	imshow("red" , planes[2] );

	namedWindow("green" , 0);
	imshow("green" , planes[1] );

	namedWindow("blue" , 0);
	imshow("blue" , planes[0] );
	waitKey(0);
}
