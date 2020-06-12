#include "../include/GroundTruthEvaluator.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
#include <vector>
#include <string>

#include <iostream>

GroundTruthEvaluator::GroundTruthEvaluator(){
    this->result = vector<Point2f>(2, Point2f(0,0));
}


GroundTruthEvaluator::GroundTruthEvaluator(float screenWidthMM,
                         float screenHeightMM,
                         const Cropper& initialDiscard,
                         const Cropper& topLeftRegion,
                         const Cropper& topRightRegion,
                         const Cropper& bottomRightRegion,
                         const Cropper& bottomLeftRegion,
                         const CornerDetector& singleCorner,
                         const Warper& warper,
                         const BlobDetector& blobDetector){

    this->initialDiscard = initialDiscard;
    this->topLeftRegion = topLeftRegion;
    this->topRightRegion = topRightRegion;
    this->bottomRightRegion = bottomRightRegion;
    this->bottomLeftRegion = bottomLeftRegion;
    this->warper = warper;
    this->singleCorner = singleCorner;
    this->blobDetector = blobDetector;
    this->result = vector<Point2f>(2, Point2f(0,0));

}

void GroundTruthEvaluator::setScreenDimMM(float screenWidthMM, float screenHeightMM){
    this->screenWidthMM = screenWidthMM;
    this->screenHeightMM = screenHeightMM;
}

void GroundTruthEvaluator::setCroppers(const Cropper& initialDiscard,
                     const Cropper& topLeftRegion,
                     const Cropper& topRightRegion,
                     const Cropper& bottomRightRegion,
                     const Cropper& bottomLeftRegion){

    this->initialDiscard = initialDiscard;
    this->topLeftRegion = topLeftRegion;
    this->topRightRegion = topRightRegion;
    this->bottomRightRegion = bottomRightRegion;
    this->bottomLeftRegion = bottomLeftRegion;

}

void GroundTruthEvaluator::setInitialDiscard(const Cropper& initialDiscard){
    this->initialDiscard = initialDiscard;
}
void GroundTruthEvaluator::setTopLeftRegion(const Cropper& topLeftRegion){
    this->topLeftRegion = topLeftRegion;
}

void GroundTruthEvaluator::setTopRightRegion(const Cropper& topRightRegion){
    this->topRightRegion = topRightRegion;
}

void GroundTruthEvaluator::setBottomRightRegion(const Cropper& bottomRightRegion){
    this->bottomRightRegion = bottomRightRegion;
}

void GroundTruthEvaluator::setBottomLeftRegion(const Cropper& bottomLeftRegion){
    this->bottomLeftRegion = bottomLeftRegion;
}


void GroundTruthEvaluator::setSingleCorner(const CornerDetector& singleCorner){
    this->singleCorner = singleCorner;
}

void GroundTruthEvaluator::setWarper(const Warper& warper){
    this->warper = warper;
}

void GroundTruthEvaluator::setBlobDetector(const BlobDetector& blobDetector){
    this->blobDetector = blobDetector;
}

void GroundTruthEvaluator::setInputImage(const Mat& inputImage){
    flip(inputImage, this->inputImage, 1);
}

void GroundTruthEvaluator::applyCroppers(){
    this->initialDiscard.setInputImage(this->inputImage);
    this->initialDiscard.doTheJob();

    this->topLeftRegion.setInputImage(this->initialDiscard.getResult());
    this->topRightRegion.setInputImage(this->initialDiscard.getResult());
    this->bottomRightRegion.setInputImage(this->initialDiscard.getResult());
    this->bottomLeftRegion.setInputImage(this->initialDiscard.getResult());
    
    this->topLeftRegion.doTheJob();
    this->topRightRegion.doTheJob();
    this->bottomRightRegion.doTheJob();
    this->bottomLeftRegion.doTheJob();
}

Point2f GroundTruthEvaluator::getCorner(Cropper& region){

    this->singleCorner.setInputImage(region.getResult());
    this->singleCorner.doTheJob();
    Point2f result = this->singleCorner.getResult()[0];
    Point2f origin;
    origin.x =  (float) (int) (region.getLeftRatio() * (region.getInputImage().cols));
    origin.y = (float) (int) (region.getTopRatio() * (region.getInputImage().rows));
    result += origin;

    
    return result;

}

void GroundTruthEvaluator::doTheJob(){

    cout << "Applying croppers" << endl;
    this->applyCroppers();
    
    cout << "Croppers applied" << endl;

    cout << "Getting top left corner" << endl;
    
    this->warper.setTopLeftSrcPoint(this->getCorner(this->topLeftRegion));
    
    cout << "Getting top right corner" << endl;
    
    this->warper.setTopRightSrcPoint(this->getCorner(this->topRightRegion));
    
    cout << "Getting top bottom right corner" << endl;
    
    this->warper.setBottomRightSrcPoint(this->getCorner(this->bottomRightRegion));

    cout << "Getting bottom left corner" << endl;

    this->warper.setBottomLeftSrcPoint(this->getCorner(this->bottomLeftRegion));

    cout << "Setting warper input image" << endl;
    
    this->warper.setInputImage(this->initialDiscard.getResult());

    cout << "warping the image" << endl;

    this->warper.doTheJob();

    cout << "Converting warped image to gray" << endl;
    Mat grayWarped;
    cvtColor(this->warper.getResult(), grayWarped, COLOR_BGR2GRAY);

    cout << "Detecting blobs" << endl;

    this->blobDetector.setInputImage(grayWarped);
    this->blobDetector.doTheJob();

    cout << "Blobs detected" << endl;
    cout << "Number of blobs detected: " << this->blobDetector.getResult().size() << endl;
    Point2f firstBlob = this->blobDetector.getResult()[0].pt;
    Point2f firstBlobMM(0,0);

    firstBlobMM.y = (firstBlob.x/grayWarped.cols)*this->screenWidthMM;
    firstBlobMM.x = (firstBlob.y/grayWarped.rows)*this->screenHeightMM;

    Point2f secondBlobMM(0,0);

    if (this->blobDetector.getResult().size() > 1){
        Point2f secondBlob = this->blobDetector.getResult()[1].pt;
        secondBlobMM.y = (secondBlob.x/this->warper.getResult().cols)*this->screenWidthMM;
        secondBlobMM.x = (secondBlob.y/this->warper.getResult().rows)*this->screenHeightMM;
    }

    this->result[0] = firstBlobMM;
    this->result[1] = secondBlobMM;



}


void GroundTruthEvaluator::saveResultAsImage(string imagePath){
    Mat resultImage;
    drawKeypoints(this->warper.getResult(), this->blobDetector.getResult(),
                  resultImage,
                  Scalar(255,0,0),
                  DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    string label;
    Point2f blobPoint;
    for (int i = 0; i < 2; i++){
        blobPoint = this->blobDetector.getResult()[i].pt;
        label = "x: ";
        label += to_string(this->result[i].x).substr(0,5);
        label += "mm ; y: ";
        label += to_string(this->result[i].y).substr(0,5);
        label += "mm";
        putText(resultImage, label,
        blobPoint, FONT_HERSHEY_PLAIN, 1.0, CV_RGB(255,0,0), 2.0);
    }
    imwrite(imagePath, resultImage);
}

void GroundTruthEvaluator::saveInputImage(string imagePath){
    imwrite(imagePath, this->inputImage);
}

vector<Point2f> GroundTruthEvaluator::getResult(){
    return this->result;
}