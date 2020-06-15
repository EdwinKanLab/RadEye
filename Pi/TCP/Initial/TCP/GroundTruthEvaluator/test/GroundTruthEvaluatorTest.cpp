#include "../../BlobDetector/include/BlobDetector.hpp"

#include "../../Cropper/include/Cropper.hpp"

#include "../../CornerDetector/include/CornerDetector.hpp"

#include "../../Warper/include/Warper.hpp"

#include "../include/GroundTruthEvaluator.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>

#include <string>
#include <utility>

using namespace cv;
using namespace std;

Point2f result [2];

Point2f getCorner(Cropper& region, CornerDetector& singleCorner){

    singleCorner.setInputImage(region.getResult());
    singleCorner.doTheJob();
    Point2f result = singleCorner.getResult()[0];
    Point2f origin;
    origin.x =  (float) (int) (region.getLeftRatio() * (region.getInputImage().cols));
    origin.y = (float) (int) (region.getTopRatio() * (region.getInputImage().rows));
    result += origin;

    
    return result;

}

void doTheJob(Cropper& initialDiscard, Cropper& topLeftRegion,
Cropper& topRightRegion, Cropper& bottomRightRegion, Cropper& bottomLeftRegion,
CornerDetector& singleCorner, Warper& warper, BlobDetector& blobDetector,
Mat& inputImage, float screenWidthMM, float screenHeightMM){

    cout << "Applying croppers" << endl;
    // this->applyCroppers();
    initialDiscard.setInputImage(inputImage);
    initialDiscard.doTheJob();

    topLeftRegion.setInputImage(initialDiscard.getResult());
    topRightRegion.setInputImage(initialDiscard.getResult());
    bottomRightRegion.setInputImage(initialDiscard.getResult());
    bottomLeftRegion.setInputImage(initialDiscard.getResult());
    
    topLeftRegion.doTheJob();
    topRightRegion.doTheJob();
    bottomRightRegion.doTheJob();
    bottomLeftRegion.doTheJob();
    
    cout << "Croppers applied" << endl;

    cout << "Getting top left corner" << endl;
    
    warper.setTopLeftSrcPoint(getCorner(topLeftRegion, singleCorner));
    
    cout << "Getting top right corner" << endl;
    
    warper.setTopRightSrcPoint(getCorner(topRightRegion, singleCorner));
    
    cout << "Getting top bottom right corner" << endl;
    
    warper.setBottomRightSrcPoint(getCorner(bottomRightRegion, singleCorner));

    cout << "Getting bottom left corner" << endl;

    warper.setBottomLeftSrcPoint(getCorner(bottomLeftRegion, singleCorner));

    cout << "Setting warper input image" << endl;
    
    warper.setInputImage(initialDiscard.getResult());

    cout << "warping the image" << endl;

    warper.doTheJob();
    warper.saveResultAsImage("../warpedImage.jpg");
    // cout << "Converting warped image to gray" << endl;
    // Mat grayWarped;
    // cvtColor(warper.getResult(), grayWarped, COLOR_BGR2GRAY);
    // imwrite("../warpedGray.jpg", grayWarped);

    cout << "Detecting blobs" << endl;

    blobDetector.setInputImage(warper.getResult());
    blobDetector.saveInputImage("../GTEBlobInput.jpg");
    blobDetector.doTheJob();

    cout << "Blobs detected" << endl;
    cout << "Number of blobs detected: " << blobDetector.getResult().size() << endl;
    Point2f firstBlobMM(-1,-1);
    Point2f secondBlobMM(-1,-1);

    if (blobDetector.getResult().size()>= 1){
        Point2f firstBlob = blobDetector.getResult()[0].pt;
        firstBlobMM.y = (1-(firstBlob.x/warper.getResult().cols))*screenWidthMM;
        firstBlobMM.x = (firstBlob.y/warper.getResult().rows)*screenHeightMM;

        if (blobDetector.getResult().size() > 1){
            Point2f secondBlob = blobDetector.getResult()[1].pt;
            secondBlobMM.y = (secondBlob.x/warper.getResult().cols)*screenWidthMM;
            secondBlobMM.x = (secondBlob.y/warper.getResult().rows)*screenHeightMM;
        }
    }
    else{
        cout << "No blobs found. Try again." << endl;
    }

    result[0] = firstBlobMM;
    result[1] = secondBlobMM;

}




int main(){
    
    Mat image;
    image = imread("../100images/raspicam_cv_image_"+
        to_string(0)+".jpg", IMREAD_COLOR);

    ///////////////////////////////////////////////////////////////////////////
    float screenWidthMM = 267;
    float screenHeightMM = 180.5;

    ///////////////////////////////////////////////////////////////////////////

    Cropper initialDiscard(0.18, 0.98, 0.03, 0.88);
    Cropper topLeftRegion(0.05, 0.15, 0.0, 0.15);
    Cropper topRightRegion(0.0, 0.10, 0.9, 1);
    Cropper bottomLeftRegion(0.85, 1, 0.01, 0.15);
    Cropper bottomRightRegion(0.85, 0.98, 0.85, 1);

    Cropper initialDiscardB(0.18, 0.98, 0.12, 0.97);
    Cropper topLeftRegionB(0, 0.10, 0.0, 0.10);
    Cropper topRightRegionB(0.05, 0.15, 0.85, 1);
    Cropper bottomLeftRegionB(0.85, 0.98, 0, 0.15);
    Cropper bottomRightRegionB(0.85, 1, 0.85, 0.99);
    ///////////////////////////////////////////////////////////////////////////
    
    CornerDetector singleCorner(1,0.1,100);
    
    ///////////////////////////////////////////////////////////////////////////

    Warper warper;

    initialDiscardB.setInputImage(image);
    initialDiscardB.doTheJob();

    topLeftRegionB.setInputImage(initialDiscardB.getResult());
    topRightRegionB.setInputImage(initialDiscardB.getResult());
    bottomRightRegionB.setInputImage(initialDiscardB.getResult());
    bottomLeftRegionB.setInputImage(initialDiscardB.getResult());
    
    topLeftRegionB.doTheJob();
    topRightRegionB.doTheJob();
    bottomRightRegionB.doTheJob();
    bottomLeftRegionB.doTheJob();
    
    warper.setTopLeftSrcPoint(getCorner(topLeftRegionB, singleCorner));
    warper.setTopRightSrcPoint(getCorner(topRightRegionB, singleCorner));
    warper.setBottomRightSrcPoint(getCorner(bottomRightRegionB, singleCorner));
    warper.setBottomLeftSrcPoint(getCorner(bottomLeftRegionB, singleCorner));
    warper.setInputImage(initialDiscardB.getResult());
    warper.doTheJob();

    ///////////////////////////////////////////////////////////////////////////

    BlobDetector blobDetector;
    blobDetector.setThresholdParams(200, 255, 10);
    blobDetector.setColorParams(true, 255);
    blobDetector.setAreaParams(false, 0, 2);
    blobDetector.setCircularityParams(false, 0, 1);
    blobDetector.setInertiaParams(true, 0.2, 1);
    blobDetector.setConvexityParams(false, 0.0, 1);
    blobDetector.setMinRepeatability(2);
    blobDetector.setMinDistBetweenBlobs(0);

    ///////////////////////////////////////////////////////////////////////////
    blobDetector.setInputImage(warper.getResult());
    blobDetector.doTheJob();
    cout << "Found "<< blobDetector.getResult().size() << " blobs." << endl;
    blobDetector.saveInputImage("../blobInput.jpg");
    blobDetector.saveResultAsImage("../Results/blobDetector.jpg");

    ///////////////////////////////////////////////////////////////////////////
    // doTheJob(initialDiscardB, topLeftRegionB, topRightRegionB, bottomRightRegionB,
    // bottomLeftRegionB, singleCorner, warper, blobDetector, image, screenWidthMM,
    // screenHeightMM);
    // BlobDetector blobDetector2 = BlobDetector();
    // blobDetector2.setThresholdParams(200, 255, 10);
    // blobDetector2.setColorParams(true, 255);
    // blobDetector2.setAreaParams(false, 0, 2);
    // blobDetector2.setCircularityParams(false, 0, 1);
    // blobDetector2.setInertiaParams(true, 0.2, 1);
    // blobDetector2.setConvexityParams(false, 0.0, 1);
    // blobDetector2.setMinRepeatability(2);
    // blobDetector2.setMinDistBetweenBlobs(0);
    GroundTruthEvaluator groundTruthEvaluator = GroundTruthEvaluator();
    groundTruthEvaluator.setBlobDetector(blobDetector);
    groundTruthEvaluator.blobDetector.setThresholdParams(200, 255, 10);
    groundTruthEvaluator.blobDetector.setColorParams(true, 255);
    groundTruthEvaluator.blobDetector.setAreaParams(false, 0, 2);
    groundTruthEvaluator.blobDetector.setCircularityParams(false, 0, 1);
    groundTruthEvaluator.blobDetector.setInertiaParams(true, 0.2, 1);
    groundTruthEvaluator.blobDetector.setConvexityParams(false, 0.0, 1);
    groundTruthEvaluator.blobDetector.setMinRepeatability(2);
    groundTruthEvaluator.blobDetector.setMinDistBetweenBlobs(0);
    groundTruthEvaluator.setScreenDimMM(screenWidthMM, screenHeightMM);

    groundTruthEvaluator.setCroppers(initialDiscardB, topLeftRegionB,
                                     topRightRegionB, bottomRightRegionB,
                                     bottomLeftRegionB);

    
    groundTruthEvaluator.setSingleCorner(singleCorner);
    groundTruthEvaluator.setWarper(warper);
    

    // ///////////////////////////////////////////////////////////////////////////
    groundTruthEvaluator.blobDetector.setInputImage(warper.getResult());
    groundTruthEvaluator.blobDetector.doTheJob();
    cout << "Found "<< groundTruthEvaluator.blobDetector.getResult().size() << " blobs." << endl;
    groundTruthEvaluator.blobDetector.saveInputImage("../GTEblobInput2.jpg");
    groundTruthEvaluator.blobDetector.saveResultAsImage("../Results/GTEblobDetector2.jpg");

    groundTruthEvaluator.setInputImage(image);
    groundTruthEvaluator.doTheJob();
    
    groundTruthEvaluator.saveInputImage("../Results/"+
    to_string(0)+"a_inputImage.jpg");
    
    // groundTruthEvaluator.saveResultAsImage("../Results/"+
    // to_string(0)+"b_groundTruthImage.jpg");
    
    ///////////////////////////////////////////////////////////////////////////
}