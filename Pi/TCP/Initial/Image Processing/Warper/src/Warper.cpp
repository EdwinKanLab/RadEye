#include "../include/Warper.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <string>
using namespace std;
using namespace cv;

// #include <iostream>

Warper::Warper(){
    this->topLeftSrcPoint = Point2f(0,0);
    this->topRightSrcPoint = Point2f(0,0);
    this->bottomRightSrcPoint = Point2f(0,0);
    this->bottomLeftSrcPoint = Point2f(0,0);
    
    this->setDstPoints();
    this->setSrcPoints();
}

Warper::Warper(Point2f topLeftSrcPoint, Point2f topRightSrcPoint,
           Point2f bottomRightSrcPoint, Point2f bottomLeftSrcPoint){
    
    this->topLeftSrcPoint = topLeftSrcPoint;
    this->topRightSrcPoint = topRightSrcPoint;
    this->bottomRightSrcPoint = bottomRightSrcPoint;
    this->bottomLeftSrcPoint = bottomLeftSrcPoint;
    
    this->setDstPoints();
    this->setSrcPoints();
}

Warper::Warper(Point2f srcPoints[4]){
    this->topLeftSrcPoint = srcPoints[0];
    this->topRightSrcPoint = srcPoints[1];
    this->bottomRightSrcPoint = srcPoints[2];
    this->bottomLeftSrcPoint = srcPoints[3];
    
    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setTopLeftSrcPoint(Point2f topLeftSrcPoint){
    this->topLeftSrcPoint = topLeftSrcPoint;


    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setTopRightSrcPoint(Point2f topRightSrcPoint){

    this->topRightSrcPoint = topRightSrcPoint;


    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setBottomLeftSrcPoint(Point2f bottomLeftSrcPoint){

    this->bottomLeftSrcPoint = bottomLeftSrcPoint;

    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setBottomRightSrcPoint(Point2f bottomRightSrcPoint){

    this->bottomRightSrcPoint = bottomRightSrcPoint;

    
    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setAllSrcPoints(Point2f topLeftSrcPoint, Point2f topRightSrcPoint,
           Point2f bottomRightSrcPoint, Point2f bottomLeftSrcPoint){
    
    this->topLeftSrcPoint = topLeftSrcPoint;
    this->topRightSrcPoint = topRightSrcPoint;
    this->bottomRightSrcPoint = bottomRightSrcPoint;
    this->bottomLeftSrcPoint = bottomLeftSrcPoint;
    
    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setAllSrcPoints(Point2f srcPoints[4]){
    this->topLeftSrcPoint = srcPoints[0];
    this->topRightSrcPoint = srcPoints[1];
    this->bottomRightSrcPoint = srcPoints[2];
    this->bottomLeftSrcPoint = srcPoints[3];
    
    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setAllSrcPoints(const vector<Point2f>& srcPoints){
    
    this->topLeftSrcPoint = srcPoints[0];
    this->topRightSrcPoint = srcPoints[1];
    this->bottomRightSrcPoint = srcPoints[2];
    this->bottomLeftSrcPoint = srcPoints[3];
    
    this->setDstPoints();
    this->setSrcPoints();
}

void Warper::setInputImage(const Mat& inputImage){
    this->inputImage = inputImage;
}

void Warper::doTheJob(){

    Mat M = getPerspectiveTransform(this->srcPoints, this->dstPoints);

    Size warpedSize(this->bottomRightDstPoint.x, this->bottomRightDstPoint.y);

    this->result = Mat(warpedSize, this->inputImage.type());
    warpPerspective(this->inputImage, this->result, M, warpedSize);
}


void Warper::saveResultAsImage(string imagePath){

    imwrite(imagePath, this->result);

}
void Warper::saveInputImage(string imagePath){
    imwrite(imagePath, this->inputImage);
}

Mat Warper::getResult(){
    return this->result;
}

void Warper::setDstPoints(){
    double topWidth = norm(this->topLeftSrcPoint - this->topRightSrcPoint);
    double bottomWidth = norm(this->bottomLeftSrcPoint - this->bottomRightSrcPoint);
    double leftHeight = norm(this->topLeftSrcPoint  - this->bottomLeftSrcPoint);
    double rightHeight = norm(this->topRightSrcPoint  - this->bottomRightSrcPoint);

    int maxHeight = (int) round(max(leftHeight, rightHeight));
    int maxWidth = (int) round(max(topWidth, bottomWidth));

    this->topLeftDstPoint = Point2f(0,0);
    this->topRightDstPoint = Point2f(maxWidth,0);
    this->bottomRightDstPoint  = Point2f(maxWidth, maxHeight);
    this->bottomLeftDstPoint  = Point2f(0, maxHeight);

    this->dstPoints[0] = this->topLeftDstPoint;
    this->dstPoints[1] = this->topRightDstPoint;
    this->dstPoints[2] = this->bottomRightDstPoint;
    this->dstPoints[3] = this->bottomLeftDstPoint;

}

void Warper::setSrcPoints(){
    this->srcPoints[0] = this->topLeftSrcPoint;
    this->srcPoints[1] = this->topRightSrcPoint;
    this->srcPoints[2] = this->bottomRightSrcPoint;
    this->srcPoints[3] = this->bottomLeftSrcPoint;
}