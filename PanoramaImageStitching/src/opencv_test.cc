#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using std::vector;


int main() {
    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();

    //Mat img_1 = imread("../test/TEST-ImageData(1)/1.bmp");
    //Mat img_2 = imread("../test/TEST-ImageData(1)/2.bmp");

    Mat img_1 = imread("1.jpeg");
    Mat img_2 = imread("2.jpeg");

    //Detect the keypoints
    vector<KeyPoint> keypoints_1, keypoints_2;
    f2d->detect(img_1, keypoints_1);
    f2d->detect(img_2, keypoints_2);
    //Calculate descriptors (feature vectors)
    Mat descriptors_1, descriptors_2;
    f2d->compute(img_1, keypoints_1, descriptors_1);
    f2d->compute(img_2, keypoints_2, descriptors_2);    
    //Matching descriptor vector using BFMatcher
    BFMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_1, descriptors_2, matches);
    //绘制匹配出的关键点
    Mat img_matches;
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);
    imshow("【match图】", img_matches);
    //等待任意按键按下
    waitKey(0);
}
    // imshow("1.bmp", img_1);
    // imshow("2.bmp", img_2);

    // vector<KeyPoint> KeyPoint_1, KeyPoint_2;
    // f2d->detect(img_1, KeyPoint_1);
    // f2d->detect(img_2, KeyPoint_2);

    // Mat descriptor_1, descriptor_2;
    // f2d->compute(img_1, KeyPoint_1, descriptor_1);
    // f2d->compute(img_2, KeyPoint_2, descriptor_2);

    // BFMatcher matcher;
    // vector<DMatch> matches;
    // matcher.match(descriptor_1, descriptor_2, matches);
    // Mat img_matches;
    // drawMatches(img_1, KeyPoint_1, img_2, KeyPoint_2, matches, img_matches);
    // imshow(" [Match图]", img_matches);
    // waitKey(0);
