#include "../include/BlobDetector.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
#include <vector>
#include <string>

// #include <iostream>

BlobDetector::BlobDetector(){
    this->updateDetector();
}

void BlobDetector::updateDetector(){
    this->detector = SimpleBlobDetector::create(this->params);

}

void BlobDetector::setThresholdParams(int minThreshold, int maxThreshold,
                                      float thresholdStep){
    this->params.minThreshold = minThreshold;
    this->params.maxThreshold = maxThreshold;
    this->params.thresholdStep = thresholdStep;

    this->updateDetector();
}

void BlobDetector::setColorParams(bool filterByColor, uchar blobColor){
    this->params.filterByColor = filterByColor;
    this->params.blobColor = blobColor;

    this->updateDetector();
}

void BlobDetector::setAreaParams(bool filterByArea, float minArea, float maxArea){
    this->params.filterByArea = filterByArea;
    this->params.minArea = minArea;
    this->params.maxArea = maxArea;

    this->updateDetector();
}

void BlobDetector::setCircularityParams(bool filterByCircularity, float minCircularity,
                              float maxCircularity){
    
    this->params.filterByCircularity = filterByCircularity;
    this->params.minCircularity = minCircularity;
    this->params.maxCircularity = maxCircularity;

    this->updateDetector();
}

void BlobDetector::setInertiaParams(bool filterByInertia, float minInertiaRatio,
                          float maxInertiaRatio){
    
    this->params.filterByInertia = filterByInertia;
    this->params.minInertiaRatio = minInertiaRatio;
    this->params.maxInertiaRatio = maxInertiaRatio;

    this->updateDetector();
}

void BlobDetector::setConvexityParams(bool filterByConvexity, float minConvexity,
                            float maxConvexity){

    this->params.filterByConvexity = filterByConvexity;
    this->params.minConvexity = minConvexity;
    this->params.maxConvexity = maxConvexity;

    this->updateDetector();
}

void BlobDetector::setMinRepeatability(size_t minRepeatability){
    this->params.minRepeatability = minRepeatability;

    this->updateDetector();
}

void BlobDetector::setMinDistBetweenBlobs(float minDistBetweenBlobs){
    this->params.minDistBetweenBlobs = minDistBetweenBlobs;

    this->updateDetector();
}


void BlobDetector::setInputImage(const Mat& inputImage){
    this->inputImage = inputImage;
}

void BlobDetector::doTheJob(){
    this->detector->detect(this->inputImage, this->result);
}


void BlobDetector::saveResultAsImage(string imagePath){
    Mat resultImage;
    drawKeypoints(this->inputImage, this->result, resultImage,
                  Scalar(0,255,0),
                  DrawMatchesFlags::DRAW_RICH_KEYPOINTS);


    imwrite(imagePath, resultImage);

}
void BlobDetector::saveInputImage(string imagePath){
    imwrite(imagePath, this->inputImage);
}

vector<KeyPoint> BlobDetector::getResult(){
    return this->result;
}