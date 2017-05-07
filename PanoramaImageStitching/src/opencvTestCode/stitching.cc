#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//计算原始图像点位在经过矩阵变换后在目标图像上对应位置
Point2f getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri);

int main(int argc, char *argv[])
{
    cout << "按顺序呢输入"  << endl;
    Mat left, right;
    string img1(argv[1]), img2(argv[2]), img3(argv[3]), img4(argv[4]);
    vector<string> imgv{img1, img2, img3, img4};
    vector<Mat> imgvv;
	Mat image01,image02;
	image01 = imread(imgv[0]);

    for (int index = 0; index < 3; ++index) {
        if (index != 0) {
            image01 = imgvv[0];
        } else {
            imgv.erase(imgv.begin());
        }
        image02 = imread(imgv[0]);
        imgvv.push_back(image02);
        imshow("l", image01);
        imshow("r", image02);
        waitKey();
	    if (image01.empty() || image02.empty())
            return -1;
	    double time = getTickCount();
	    //灰度图转换
	    Mat image1, image2;
	    cvtColor(image01, image1, CV_RGB2GRAY);
	    cvtColor(image02, image2, CV_RGB2GRAY);

	    //提取特征点
	    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
	    vector<KeyPoint> keyPoint1, keyPoint2;
	    f2d->detect(image1, keyPoint1);
	    f2d->detect(image2, keyPoint2);

	    //特征点描述，为下边的特征点匹配做准备
	    Mat imageDesc1, imageDesc2;
	    f2d->compute(image1, keyPoint1, imageDesc1);
	    f2d->compute(image2, keyPoint2, imageDesc2);

	    //获得匹配特征点，并提取最优配对
	    FlannBasedMatcher matcher;
	    vector<DMatch> matchePoints;
	    matcher.match(imageDesc1, imageDesc2, matchePoints, Mat());
	    if (matchePoints.size() < 10)
	    {
		    return 0;
	    }

	    sort(matchePoints.begin(), matchePoints.end()); //特征点排序，opencv按照匹配点准确度排序
	    //获取排在前N个的最优匹配特征点
	    vector<Point2f> imagePoints1, imagePoints2;
	    for (int i = 0; i<10; i++) {
		    imagePoints1.push_back(keyPoint1[matchePoints[i].queryIdx].pt);
		    imagePoints2.push_back(keyPoint2[matchePoints[i].trainIdx].pt);
	    }
	    //获取图像1到图像2的投影映射矩阵，尺寸为3*3
	    Mat homo = findHomography(imagePoints1, imagePoints2, CV_RANSAC);
	    Mat adjustMat = (Mat_<double>(3, 3) << 1.0, 0, image01.cols, 0, 1.0, 0, 0, 0, 1.0);//向后偏移image01.cols矩阵
	    Mat adjustHomo = adjustMat*homo;//矩阵相乘，先偏移

	    //获取最强配对点（就是第一个配对点）在原始图像和矩阵变换后图像上的对应位置，用于图像拼接点的定位
	    Point2f originalLinkPoint, targetLinkPoint, basedImagePoint;
	    originalLinkPoint = keyPoint1[matchePoints[0].queryIdx].pt;
	    targetLinkPoint = getTransformPoint(originalLinkPoint, adjustHomo);
	    basedImagePoint = keyPoint2[matchePoints[0].trainIdx].pt;
	    //图像配准
	    Mat imageTransform;
	    //将图片1进行映射到图像2，本来映射后x值为负值，但是把映射矩阵向后偏移image01.cols矩阵
	    //我们很难判断出拼接后图像的大小尺寸，为了尽可能保留原来的像素，我们尽可能的大一些，对于拼接后的图片可以进一步剪切无效或者不规则的边缘
	    warpPerspective(image01, imageTransform, adjustMat*homo, Size(image02.cols + image01.cols+10, image02.rows));
	    //在最强匹配点的位置处衔接，最强匹配点左侧是图1，右侧是图2，这样直接替换图像衔接不好，光线有突变
	    //Mat ROIMat = image02(Rect(Point(basedImagePoint.x, 0), Point(image02.cols, image02.rows)));
	    //ROIMat.copyTo(Mat(imageTransform1, Rect(targetLinkPoint.x, 0, image02.cols - basedImagePoint.x + 1, image02.rows)));

	    //在最强匹配点左侧的重叠区域进行累加，是衔接稳定过渡，消除突变
	    Mat image1Overlap, image2Overlap; //图1和图2的重叠部分
	    image1Overlap = imageTransform(Rect(Point(targetLinkPoint.x - basedImagePoint.x, 0), Point(targetLinkPoint.x, image02.rows)));
	    image2Overlap = image02(Rect(0, 0, image1Overlap.cols, image1Overlap.rows));
	    Mat image1ROICopy = image1Overlap.clone();  //复制一份图1的重叠部分
	    for (int i = 0; i<image1Overlap.rows; i++) {
		    for (int j = 0; j<image1Overlap.cols; j++) {
			    double weight;
			    weight = (double)j / image1Overlap.cols;  //随距离改变而改变的叠加系数
			    image1Overlap.at<Vec3b>(i, j)[0] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[0] + weight*image2Overlap.at<Vec3b>(i, j)[0];
			    image1Overlap.at<Vec3b>(i, j)[1] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[1] + weight*image2Overlap.at<Vec3b>(i, j)[1];
			    image1Overlap.at<Vec3b>(i, j)[2] = (1 - weight)*image1ROICopy.at<Vec3b>(i, j)[2] + weight*image2Overlap.at<Vec3b>(i, j)[2];
		    }
	    }
	    Mat ROIMat = image02(Rect(Point(image1Overlap.cols, 0), Point(image02.cols, image02.rows)));  //图2中不重合的部分
	    ROIMat.copyTo(Mat(imageTransform, Rect(targetLinkPoint.x, 0, ROIMat.cols, image02.rows))); //不重合的部分直接衔接上去

	    time = getTickCount() - time;
	    time /= getTickFrequency();
        std::cout << "match time=" << time << std::endl;
        imgvv[0] = imageTransform;
        imgv.erase(imgv.begin());
	}
	namedWindow("拼接结果", 0);
	imshow("拼接结果", imgvv[0]);
	imwrite("Result.jpg",imgvv[0]);
	waitKey();
	return 0;
}

//计算原始图像点位在经过矩阵变换后在目标图像上对应位置
Point2f getTransformPoint(const Point2f originalPoint, const Mat &transformMaxtri)
{
	Mat originelP, targetP;
	originelP = (Mat_<double>(3, 1) << originalPoint.x, originalPoint.y, 1.0);
	targetP = transformMaxtri*originelP;
	float x = targetP.at<double>(0, 0) / targetP.at<double>(2, 0);
	float y = targetP.at<double>(1, 0) / targetP.at<double>(2, 0);
	return Point2f(x, y);
}
