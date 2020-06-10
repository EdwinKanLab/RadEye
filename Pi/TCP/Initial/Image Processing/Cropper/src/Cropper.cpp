#include "../include/Cropper.hpp"
#include <opencv2/imgcodecs.hpp>
using namespace std;
using namespace cv;
// #include <iostream>


Cropper::Cropper(){
    ;
}

Cropper::Cropper(float topRatio, float bottomRatio, float leftRatio, float rightRatio){
    this->topRatio = topRatio;
    this->bottomRatio = bottomRatio;
    this->leftRatio = leftRatio;
    this->rightRatio = rightRatio;
    this->updateHeightAndWidth();

}


void Cropper::setTopRatio(float topRatio){
    this->topRatio = topRatio;
    this->updateHeightAndWidth();
}

void Cropper::setBottomRatio(float bottomRatio){
    this->bottomRatio = bottomRatio;
    this->updateHeightAndWidth();
}

void Cropper::setLeftRatio(float leftRatio){
    this->leftRatio = leftRatio;
    this->updateHeightAndWidth();
}

void Cropper::setRightRatio(float rightRatio){
    this->rightRatio = rightRatio;
    this->updateHeightAndWidth();
}

void Cropper::setAllRatio(float topRatio, float bottomRatio, float leftRatio, float rightRatio){
    this->topRatio = topRatio;
    this->bottomRatio = bottomRatio;
    this->leftRatio = leftRatio;
    this->rightRatio = rightRatio;
    this->updateHeightAndWidth();
}

void Cropper::setInputImage(const Mat& inputImage){
    
    
    if (inputImage.rows == this->inputImage.rows &&
        inputImage.cols == this->inputImage.cols){
            this->inputImage = inputImage;
        }
    
    else{
        this->inputImage = inputImage;
        this->updateHeightAndWidth();
    }


}

void Cropper::doTheJob(){
    // cout<< "height start: " << this->height.start << endl;
    // cout << "height end: "<< this->height.end<< endl;
    // cout << "width start: " << this->width.start << endl;
    // cout << "width end: " <<this->width.end << endl;
    this->result = this->inputImage(this->height, this->width);
}


void Cropper::saveResultAsImage(string imagePath){
    imwrite(imagePath, this->result);

}

void Cropper::saveInputImage(string imagePath){
    imwrite(imagePath, this->inputImage);
}

Mat Cropper::getResult(){
    return this->result;
}

float Cropper::getTopRatio(){
    return this->topRatio;
}

float Cropper::getBottomRatio(){
    return this->bottomRatio;
}

float Cropper::getLeftRatio(){
    return this->leftRatio;
}

float Cropper::getRightRatio(){
    return this->rightRatio;
}

Mat Cropper::getInputImage(){
    return this->inputImage;
}

void Cropper::updateHeightAndWidth(){
    // cout << (this->inputImage).rows << endl;
    int topIndex = (int) (this->inputImage).rows * this->topRatio;
    int bottomIndex = (int) (this->inputImage).rows * this->bottomRatio;
    int leftIndex = (int) (this->inputImage).cols * this->leftRatio;
    int rightIndex = (int) (this->inputImage).cols * this->rightRatio;

    this->height = Range(topIndex, bottomIndex);
    this->width = Range(leftIndex, rightIndex);
}