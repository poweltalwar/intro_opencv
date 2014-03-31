#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std ;

void negative()
{
	char* path = "/home/powel/codes/powel2/src1/dp1.png" ;
	Mat image= imread(path,3);
	cout<< "image depth		"<<image.depth() ;

	Mat im2 ,planes[3];

	planes[0] = Mat::ones(image.rows , image.cols , 0) * 255;
	planes[1] = planes[2] = planes[0] ;
	merge(planes,3, im2);



	namedWindow("original" , 0);
	imshow("original" ,image);

    im2 = im2 - image ;
    //planes[0] = planes[0] - image ;

	namedWindow("negative" , 0);
	imshow("negative" ,im2);

	waitKey(0);
}
