#pragma once
#include <opencv2/core/mat.hpp>

#include <string>
#include <vector>

#include "../../Cropper/include/Cropper.hpp"
#include "../../CornerDetector/include/CornerDetector.hpp"
#include "../../Warper/include/Warper.hpp"
#include "../../BlobDetector/include/BlobDetector.hpp"




class GroundTruthEvaluator{
private:
    cv::Mat inputImage;
 	cv::Point2f result[2];

    float screenWidthMM;
    float screenHeightMM;

    Cropper initialDiscard;
    Cropper topLeftRegion;
    Cropper topRightRegion;
    Cropper bottomRightRegion;
    Cropper bottomLeftRegion;

    CornerDetector singleCorner;

    Warper warper;
    BlobDetector blobDetector;

    void applyCroppers();
    cv::Point2f getCorner(Cropper& region);
    
public:

    GroundTruthEvaluator();
    GroundTruthEvaluator(float screenWidthMM,
                         float screenHeightMM,
                         Cropper& initialDiscard,
                         Cropper& topLeftRegion,
                         Cropper& topRightRegion,
                         Cropper& bottomRightRegion,
                         Cropper& bottomLeftRegion,
                         CornerDetector& singleCorner,
                         Warper& warper,
                         BlobDetector& blobDetector);
    
    void setScreenDimMM(float screenWidthMM, float screenHeightMM);
    
    void setCroppers(Cropper& initialDiscard,
                     Cropper& topLeftRegion,
                     Cropper& topRightRegion,
                     Cropper& bottomRightRegion,
                     Cropper& bottomLeftRegion);
    
    
    void setInitialDiscard(Cropper& initialDiscard);
    void setTopLeftRegion(Cropper& topLeftRegion);
    void setTopRightRegion(Cropper& topRightRegion);
    void setBottomRightRegion(Cropper& bottomRightRegion);
    void setBottomLeftRegion(Cropper& bottomLeftRegion);


    void setSingleCorner(CornerDetector& singleCorner);
    void setWarper(Warper& warper);
    void setBlobDetector(BlobDetector& blobDetector);

    void setInputImage(cv::Mat& inputImage);
    void doTheJob();
    void saveResultAsImage(std::string imagePath);
    void saveInputImage(std::string imagePath);
    
    cv::Point2f* getResult();

};