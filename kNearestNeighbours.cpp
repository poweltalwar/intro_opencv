#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;


Mat getDistance( Mat clusters, Mat point );
Mat getIdx( Mat dists );
void Clustering( Mat points, int K, int iterMax, Mat& centers, Mat& dist, Mat& idx );


void getNearestCluster(Mat points, Mat centers, Mat& index)
{
	Mat dists(points.rows, centers.rows, CV_32F), temp ;
	cout << dists.rows << "\t" << dists.cols <<  endl ;

	for(int k = 0; k < centers.rows; k++)
	{
		getDistance(points, centers.row(k)).copyTo(dists.col(k)) ;
		transpose(dists.col(k), temp);
		getIdx(temp).copyTo(index.row(k));
	}
}


void kNN(Mat points, int K)
{
	int length = points.rows, cols = points.cols, numClusters = 10, maxIter = 2 ;

	Mat centers(numClusters, cols, CV_32F);
	Mat distClusters;
	Mat idx(length, 1, CV_32F);
	Mat data = points.clone();

	Clustering(data, numClusters, maxIter, centers, distClusters, idx) ;

	Mat index = Mat::ones(length, 1, CV_32F);
	getNearestCluster(centers, points, index);

}
