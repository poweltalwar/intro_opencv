#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;


Mat getInitCenters( int K, int length, int cols )
{
	RNG random(0xFFFFFFFF);
	Mat centers(K, cols, CV_32F);
	float num;

	for(int i = 0; i < K; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			num = random.uniform( (double)0, (double)1 ) ;
			centers.at<float>(i,j) = num;
		}
	}

	return centers;
}


Mat getDistance( Mat points, Mat center )
{
	Mat dist, temp(points.rows, points.cols, CV_32F);
	int cols = points.cols;

	for(int i = 0; i < cols; i++)
	{
		subtract(points.col(i), center.at<float>(0,i), temp.col(i));
	}

	multiply(temp, temp, temp);
	reduce(temp, dist, 1, CV_REDUCE_SUM);temp.release();
	sqrt(dist,dist);

	return dist;
}


Mat getIdx( Mat dists )
{
	Mat idx = Mat::ones(dists.rows,1,CV_32F);

	double max, min;
	Point minPos, maxPos ;

	for(int i = 0; i < dists.rows; i++)
	{
		minMaxLoc( dists.row(i), &min, &max, &minPos, &maxPos );
		idx.at<float>(i,0) = (minPos).x ;
	}

	return idx;
}


void adjustClusters( Mat& centers, Mat points, Mat idx, int K )
{
	int cols = centers.cols, numEl;

	Mat mask, tempAvg(idx.rows, cols, CV_32F);

	for(int i = 0; i < K; i++)
	{
		compare(idx, i, mask, CMP_EQ);
		mask /= 255 ;

		for(int k = 0; k < cols; k++)
		{
			multiply(mask, points.col(k), tempAvg.col(k) );
		}

		numEl = countNonZero(mask) ;

		reduce(tempAvg, centers.row(i), 0, CV_REDUCE_SUM);
		centers.row(i) /= (numEl+1) ;
	}
	mask.release();
	tempAvg.release();
}


void compress(Mat points, Mat idx, Mat centers)
{
	for(int i = 0; i < points.rows; i++)
	{
		centers.row( idx.at<float>(i,0) ).copyTo(points.row(i));
	}

}


void Clustering( Mat points, int K, int iterMax, Mat& centers, Mat& distFinal, Mat& idx )
{
	int length = points.rows, cols = points.cols;

	//Mat centers(K, cols, CV_32F);
	Mat dists(length, K, CV_32F);
	//Mat idx(length, 1, CV_32F);
	//Mat distFinal;

	centers = getInitCenters(K, length, cols);

	for(int iter = 0; iter < iterMax; iter++)
	{
		for(int k = 0; k < K; k++)
		{
			getDistance(points, centers.row(k)).copyTo(dists.col(k)) ;
		}
		idx = getIdx(dists);
		adjustClusters(centers, points, idx, K) ;

		cout << "numIter = " << iter << endl ;
	}

	reduce(dists, distFinal, 1, CV_REDUCE_MIN);dists.release();
	//compress(points, idx, centers);

}

