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
 	std::vector<cv::Point2f> result;

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
                         const Cropper& initialDiscard,
                         const Cropper& topLeftRegion,
                         const Cropper& topRightRegion,
                         const Cropper& bottomRightRegion,
                         const Cropper& bottomLeftRegion,
                         const CornerDetector& singleCorner,
                         const Warper& warper,
                         const BlobDetector& blobDetector);
    
    void setScreenDimMM(float screenWidthMM, float screenHeightMM);
    
    void setCroppers(const Cropper& initialDiscard,
                     const Cropper& topLeftRegion,
                     const Cropper& topRightRegion,
                     const Cropper& bottomRightRegion,
                     const Cropper& bottomLeftRegion);
    
    
    void setInitialDiscard(const Cropper& initialDiscard);
    void setTopLeftRegion(const Cropper& topLeftRegion);
    void setTopRightRegion(const Cropper& topRightRegion);
    void setBottomRightRegion(const Cropper& bottomRightRegion);
    void setBottomLeftRegion(const Cropper& bottomLeftRegion);


    void setSingleCorner(const CornerDetector& singleCorner);
    void setWarper(const Warper& warper);
    void setBlobDetector(const BlobDetector& blobDetector);

    void setInputImage(const cv::Mat& inputImage);
    void doTheJob();
    void saveResultAsImage(std::string imagePath);
    void saveInputImage(std::string imagePath);
    
    std::vector<cv::Point2f> getResult();

};