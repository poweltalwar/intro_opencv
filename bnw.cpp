#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std ;

void bnw()
{
char* path = "/home/powel/codes/powel2/src1/dp1.png" ;
Mat im1 = imread(path,3);

// creates a duplicate image to prevent change to original image
Mat im2,im3  ;

// convert from rgb to gray
cvtColor(im1,im2,CV_RGB2GRAY);
im3 = im2.clone() ;

adaptiveThreshold(im2, im2, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY,15,0) ;

// 0 = resize the window , 1 == cannot resize the window
namedWindow("binary1", 0 );
imshow("binary1", im2);

namedWindow("original", 0 );
imshow("original", im1);

adaptiveThreshold(im3, im2, 255, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 15,0) ;

namedWindow("binary", 0 );
imshow("binary", im2);

waitKey(0);

}
