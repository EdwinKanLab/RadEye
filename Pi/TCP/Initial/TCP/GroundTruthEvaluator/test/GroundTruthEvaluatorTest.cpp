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
    blobDetector.saveResultAsImage("../Results/blobDetector.jpg");

    ///////////////////////////////////////////////////////////////////////////
    GroundTruthEvaluator groundTruthEvaluator = GroundTruthEvaluator();
    groundTruthEvaluator.setScreenDimMM(screenWidthMM, screenHeightMM);

    groundTruthEvaluator.setCroppers(initialDiscard, topLeftRegion,
                                     topRightRegion, bottomRightRegion,
                                     bottomLeftRegion);

    
    groundTruthEvaluator.setSingleCorner(singleCorner);
    groundTruthEvaluator.setWarper(warper);
    groundTruthEvaluator.setBlobDetector(blobDetector);

    ///////////////////////////////////////////////////////////////////////////


    groundTruthEvaluator.setInputImage(image);
    groundTruthEvaluator.doTheJob();
    
    groundTruthEvaluator.saveInputImage("../Results/"+
    to_string(0)+"a_inputImage.jpg");
    
    groundTruthEvaluator.saveResultAsImage("../Results/"+
    to_string(0)+"b_groundTruthImage.jpg");
    
    ///////////////////////////////////////////////////////////////////////////
}