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
    this->result[0] = Point2f(-1, -1);
    this->result[1] = Point2f(-1, -1);
}


GroundTruthEvaluator::GroundTruthEvaluator(float screenWidthMM,
                         float screenHeightMM,
                         Cropper& initialDiscard,
                         Cropper& topLeftRegion,
                         Cropper& topRightRegion,
                         Cropper& bottomRightRegion,
                         Cropper& bottomLeftRegion,
                         CornerDetector& singleCorner,
                         Warper& warper,
                         BlobDetector& blobDetector){

    this->initialDiscard = initialDiscard;
    this->topLeftRegion = topLeftRegion;
    this->topRightRegion = topRightRegion;
    this->bottomRightRegion = bottomRightRegion;
    this->bottomLeftRegion = bottomLeftRegion;
    this->warper = warper;
    this->singleCorner = singleCorner;
    this->blobDetector = blobDetector;
    this->result[0] = Point2f(-1, -1);
    this->result[1] = Point2f(-1, -1);

}

void GroundTruthEvaluator::setScreenDimMM(float screenWidthMM, float screenHeightMM){
    this->screenWidthMM = screenWidthMM;
    this->screenHeightMM = screenHeightMM;
}

void GroundTruthEvaluator::setCroppers(Cropper& initialDiscard,
                     Cropper& topLeftRegion,
                     Cropper& topRightRegion,
                     Cropper& bottomRightRegion,
                     Cropper& bottomLeftRegion){

    this->initialDiscard = initialDiscard;
    this->topLeftRegion = topLeftRegion;
    this->topRightRegion = topRightRegion;
    this->bottomRightRegion = bottomRightRegion;
    this->bottomLeftRegion = bottomLeftRegion;

}

void GroundTruthEvaluator::setInitialDiscard(Cropper& initialDiscard){
    this->initialDiscard = initialDiscard;
}
void GroundTruthEvaluator::setTopLeftRegion(Cropper& topLeftRegion){
    this->topLeftRegion = topLeftRegion;
}

void GroundTruthEvaluator::setTopRightRegion(Cropper& topRightRegion){
    this->topRightRegion = topRightRegion;
}

void GroundTruthEvaluator::setBottomRightRegion(Cropper& bottomRightRegion){
    this->bottomRightRegion = bottomRightRegion;
}

void GroundTruthEvaluator::setBottomLeftRegion(Cropper& bottomLeftRegion){
    this->bottomLeftRegion = bottomLeftRegion;
}


void GroundTruthEvaluator::setSingleCorner(CornerDetector& singleCorner){
    this->singleCorner = singleCorner;
}

void GroundTruthEvaluator::setWarper(Warper& warper){
    this->warper = warper;
    this->warper.setTopLeftSrcPoint(warper.topLeftSrcPoint);
    this->warper.setTopRightSrcPoint(warper.topRightSrcPoint);
    this->warper.setBottomRightSrcPoint(warper.bottomRightSrcPoint);
    this->warper.setBottomLeftSrcPoint(warper.bottomLeftSrcPoint);
    cout << "TL x: " << this->warper.srcPoints[0].x << " TL y: " << this->warper.srcPoints[0].y << endl;
    cout << "TR x: " << this->warper.srcPoints[1].x << " TR y: " << this->warper.srcPoints[1].y << endl;
    cout << "BR x: " << this->warper.srcPoints[2].x << " BR y: " << this->warper.srcPoints[2].y << endl;
    cout << "BL x: " << this->warper.srcPoints[3].x << " BL y: " << this->warper.srcPoints[3].y << endl;
    // this->warper.setAllSrcPoints(warper.srcPoints);
}

void GroundTruthEvaluator::setBlobDetector(BlobDetector& blobDetector){
    this->blobDetector = blobDetector;
    this->blobDetector.setThresholdParams(blobDetector.params.minThreshold,
    blobDetector.params.maxThreshold, blobDetector.params.thresholdStep);

    this->blobDetector.setColorParams(blobDetector.params.filterByColor,
    blobDetector.params.blobColor);

    this->blobDetector.setAreaParams(blobDetector.params.filterByArea,
    blobDetector.params.minArea, blobDetector.params.maxArea);

    this->blobDetector.setCircularityParams(blobDetector.params.filterByCircularity,
    blobDetector.params.minCircularity, blobDetector.params.maxCircularity);

    this->blobDetector.setInertiaParams(blobDetector.params.filterByInertia,
    blobDetector.params.minInertiaRatio, blobDetector.params.maxInertiaRatio);
    
    this->blobDetector.setConvexityParams(blobDetector.params.filterByConvexity,
    blobDetector.params.minConvexity, blobDetector.params.maxConvexity);

    this->blobDetector.setMinRepeatability(blobDetector.params.minRepeatability);
    this->blobDetector.setMinDistBetweenBlobs(blobDetector.params.minDistBetweenBlobs);
}

void GroundTruthEvaluator::setInputImage(Mat& inputImage){
    this->inputImage = inputImage;
    // flip(inputImage, this->inputImage, 1);
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

    // cout << "Applying croppers" << endl;
    this->applyCroppers();
    // this->initialDiscard.setInputImage(this->inputImage);
    // this->initialDiscard.doTheJob();

    // this->topLeftRegion.setInputImage(this->initialDiscard.getResult());
    // this->topRightRegion.setInputImage(this->initialDiscard.getResult());
    // this->bottomRightRegion.setInputImage(this->initialDiscard.getResult());
    // this->bottomLeftRegion.setInputImage(this->initialDiscard.getResult());
    
    // this->topLeftRegion.doTheJob();
    // this->topRightRegion.doTheJob();
    // this->bottomRightRegion.doTheJob();
    // this->bottomLeftRegion.doTheJob();
    
    // cout << "Croppers applied" << endl;

    // cout << "Getting top left corner" << endl;
    
    this->warper.setTopLeftSrcPoint(this->getCorner(this->topLeftRegion));
    
    // cout << "Getting top right corner" << endl;
    
    this->warper.setTopRightSrcPoint(this->getCorner(this->topRightRegion));
    
    // cout << "Getting top bottom right corner" << endl;
    
    this->warper.setBottomRightSrcPoint(this->getCorner(this->bottomRightRegion));

    // cout << "Getting bottom left corner" << endl;

    this->warper.setBottomLeftSrcPoint(this->getCorner(this->bottomLeftRegion));

    // cout << "Setting warper input image" << endl;
    
    this->warper.setInputImage(this->initialDiscard.getResult());

    // cout << "warping the image" << endl;

    this->warper.doTheJob();
    // this->warper.saveResultAsImage("../warpedImage.jpg");
    // cout << "Converting warped image to gray" << endl;
    // Mat grayWarped;
    // cvtColor(this->warper.getResult(), grayWarped, COLOR_BGR2GRAY);
    // imwrite("../warpedGray.jpg", grayWarped);

    // cout << "Detecting blobs" << endl;

    this->blobDetector.setInputImage(this->warper.getResult());
    // this->blobDetector.saveInputImage("../GTEBlobInput.jpg");
    this->blobDetector.doTheJob();

    // cout << "Blobs detected" << endl;
    // cout << "Number of blobs detected: " << this->blobDetector.getResult().size() << endl;
    Point2f firstBlobMM(-1,-1);
    Point2f secondBlobMM(-1,-1);

    if (this->blobDetector.getResult().size()>= 1){
        Point2f firstBlob = this->blobDetector.getResult()[0].pt;
        firstBlobMM.y = (1-(firstBlob.x/this->warper.getResult().cols))*this->screenWidthMM;
        firstBlobMM.x = (firstBlob.y/this->warper.getResult().rows)*this->screenHeightMM;

        if (this->blobDetector.getResult().size() > 1){
            Point2f secondBlob = this->blobDetector.getResult()[1].pt;
            secondBlobMM.y = (secondBlob.x/this->warper.getResult().cols)*this->screenWidthMM;
            secondBlobMM.x = (secondBlob.y/this->warper.getResult().rows)*this->screenHeightMM;
        }
    }
    else{
        cout << "No blobs found. Try again." << endl;
    }

    this->result[0] = firstBlobMM;
    this->result[1] = secondBlobMM;



}

void GroundTruthEvaluator::doTheJob(bool corner){
    if (corner) this->doTheJob();
    
    else{
        this->initialDiscard.setInputImage(this->inputImage);
        this->initialDiscard.doTheJob();

        this->warper.setInputImage(this->initialDiscard.getResult());

        this->warper.doTheJob();


        this->blobDetector.setInputImage(this->warper.getResult());
        this->blobDetector.doTheJob();
        Point2f firstBlobMM(-1,-1);
        Point2f secondBlobMM(-1,-1);

        if (this->blobDetector.getResult().size()>= 1){
            Point2f firstBlob = this->blobDetector.getResult()[0].pt;
            firstBlobMM.y = (1-(firstBlob.x/this->warper.getResult().cols))*this->screenWidthMM;
            firstBlobMM.x = (firstBlob.y/this->warper.getResult().rows)*this->screenHeightMM;

            if (this->blobDetector.getResult().size() > 1){
                Point2f secondBlob = this->blobDetector.getResult()[1].pt;
                secondBlobMM.y = (secondBlob.x/this->warper.getResult().cols)*this->screenWidthMM;
                secondBlobMM.x = (secondBlob.y/this->warper.getResult().rows)*this->screenHeightMM;
            }
        }
        else{
            cout << "No blobs found. Try again." << endl;
        }

        this->result[0] = firstBlobMM;
        this->result[1] = secondBlobMM;
    }

}

void GroundTruthEvaluator::saveResultAsImage(string imagePath){
    Mat resultImage;
    Mat warpedFlipped;
    flip(this->warper.getResult(), warpedFlipped, 1);
    vector<KeyPoint> keypoints;

    if (this->blobDetector.getResult().size() >= 1){
        keypoints.push_back(this->blobDetector.getResult()[0]);
    }
    if (this->blobDetector.getResult().size() > 1){
        keypoints.push_back(this->blobDetector.getResult()[1]);
    }

    for (int i = 0; i < keypoints.size(); i++){
        keypoints[i].pt.x = warpedFlipped.cols - keypoints[i].pt.x;
    }
    
    drawKeypoints(warpedFlipped, keypoints,
                  resultImage,
                  Scalar(255,0,0),
                  DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    
    string label;
    Point2f blobPoint;
    for (int i = 0; i < keypoints.size(); i++){
        blobPoint = keypoints[i].pt;
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

Point2f* GroundTruthEvaluator::getResult(){
    return this->result;
}