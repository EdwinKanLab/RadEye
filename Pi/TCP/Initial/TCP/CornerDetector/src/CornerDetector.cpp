#include "../include/CornerDetector.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
using namespace std;
using namespace cv;

CornerDetector::CornerDetector(){
    this->maxCorners = 1;
    this->qualityLevel = 0.1;
    this->minDistance = 100;
}
CornerDetector::CornerDetector(int maxCorners, double qualityLevel, double minDistance){
    this->maxCorners = maxCorners;
    this->qualityLevel = qualityLevel;
    this->minDistance = minDistance;
    this->mask = Mat();
    this->blockSize = 3;
    this->useHarrisDetector = false;
    this->k = 0.04;
}

CornerDetector::CornerDetector(int maxCorners, double qualityLevel, double minDistance, int blockSize){
    this->maxCorners = maxCorners;
    this->qualityLevel = qualityLevel;
    this->minDistance = minDistance;
    this->blockSize = blockSize;
    this->mask = Mat();
    this->useHarrisDetector = false;
    this->k = 0.04;

}

CornerDetector:: CornerDetector(int maxCorners, double qualityLevel,
                    double minDistance, cv::Mat mask, int blockSize,
                    bool useHarrisDetector, double k){
    this->maxCorners = maxCorners;
    this->qualityLevel = qualityLevel;
    this->minDistance = minDistance;
    this->mask = mask;
    this->blockSize = blockSize;
    this->useHarrisDetector = useHarrisDetector;
    this->k = k;

}

void CornerDetector::setInputImage(const Mat& inputImage){
    
    this->inputImage = inputImage;
    Mat gray;
    // cvtColor(inputImage, this->grayImage, COLOR_BGR2GRAY);
    cvtColor(inputImage, gray, COLOR_BGR2GRAY);
    Mat gBlur;
    GaussianBlur(gray, gBlur, Size(5,5), 0);
    threshold(gBlur, this->grayImage, 0, 255, THRESH_BINARY + THRESH_OTSU);
}

void CornerDetector::doTheJob(){

    goodFeaturesToTrack(this->grayImage, this->result, this->maxCorners,
    this->qualityLevel, this->minDistance, this->mask, this->blockSize,
    this->useHarrisDetector, this->k);
}


void CornerDetector::saveResultAsImage(string imagePath){
    for (int i = 0; i < this->result.size(); i++){
        circle(this->inputImage, this->result[i], 5, 255, -1);
    }

    imwrite(imagePath, this->inputImage);

}
void CornerDetector::saveInputImage(string imagePath){
    imwrite(imagePath, this->inputImage);
}

vector<Point2f> CornerDetector::getResult(){
    return this->result;
}