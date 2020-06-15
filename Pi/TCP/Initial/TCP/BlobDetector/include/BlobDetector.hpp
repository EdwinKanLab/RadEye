#pragma once
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <string>
#include <vector>

class BlobDetector{
private:
    cv::Mat inputImage;
    
    std::vector<cv::KeyPoint> result;

    cv::Ptr<cv::SimpleBlobDetector> detector;

    void updateDetector();

public:

    BlobDetector();
    
    cv::SimpleBlobDetector::Params params;
    
    void setThresholdParams(int minThreshold, int maxThreshold, float thresholdStep);

    void setColorParams(bool filterByColor, uchar blobColor);

    void setAreaParams(bool filterByArea, float minArea, float maxArea);

    void setCircularityParams(bool filterByCircularity, float minCircularity,
                              float maxCircularity);
    
    void setInertiaParams(bool filterByInertia, float minInertiaRatio,
                          float maxInertiaRatio);
    
    void setConvexityParams(bool filterByConvexity, float minConvexity,
                            float maxConvexity);
    
    void setMinRepeatability(size_t minRepeatability);

    void setMinDistBetweenBlobs(float minDistBetweenBlobs);

    
    void setInputImage(const cv::Mat& inputImage);
    void doTheJob();
    void saveResultAsImage(std::string imagePath);
    void saveInputImage(std::string imagePath);
    std::vector<cv::KeyPoint> getResult();

};