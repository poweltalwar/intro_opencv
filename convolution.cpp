#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv ;

void convolution()
{

	char* path = "/home/powel/codes/powel2/src1/dp1.png" ;
	Mat image = imread(path,0);
	Mat im = image.clone();
	int ddepth = -1 ;

	Mat k2 = Mat::eye(3,3,CV_32F) ;

	Mat kernel = Mat::ones(3,3,CV_32F);

	kernel = (-1*kernel+10*k2)/21  ;

	filter2D(image , im, ddepth, kernel);

    namedWindow("original" , 0) ;
    imshow("original",image);

    namedWindow("filtered" , 0) ;
    imshow("filtered",im);
    waitKey(0);
    return ;

}
