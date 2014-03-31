#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

void gray()
{
char* path = "/home/powel/codes/powel2/src1/dp1.png" ;
Mat im1 = imread(path,3);

// creates a duplicate image to prevent change to original image
Mat im2 = im1.clone() ;

// convert from rgb to gray
cvtColor( im2, im2, CV_RGB2GRAY );

// 0 = resize the window , 1 == cannot resize the window
namedWindow("gray", 0 );
imshow("gray", im2);

waitKey(0);

}
