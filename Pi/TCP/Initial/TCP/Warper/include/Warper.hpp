#pragma once
#include <opencv2/core/mat.hpp>
#include <string>
#include <vector>

class Warper{
private:
    cv::Mat inputImage;
    cv::Mat result;

    cv::Point2f topLeftSrcPoint;
    cv::Point2f topRightSrcPoint;
    cv::Point2f bottomRightSrcPoint;
    cv::Point2f bottomLeftSrcPoint;
    cv::Point2f srcPoints[4];

    cv::Point2f topLeftDstPoint;
    cv::Point2f topRightDstPoint;
    cv::Point2f bottomRightDstPoint;
    cv::Point2f bottomLeftDstPoint;
    cv::Point2f dstPoints[4];

    void setSrcPoints();
    void setDstPoints();
    

public:
    Warper();
    Warper(cv::Point2f topLeftSrcPoint, cv::Point2f topRightSrcPoint,
           cv::Point2f bottomRightSrcPoint, cv::Point2f bottomLeftSrcPoint);
    
    Warper(cv::Point2f srcPoints[4]);
    
    void setTopLeftSrcPoint(cv::Point2f topLeftSrcPoint);
    void setTopRightSrcPoint(cv::Point2f topRightSrcPoint);
    void setBottomLeftSrcPoint(cv::Point2f bottomLeftSrcPoint);
    void setBottomRightSrcPoint(cv::Point2f bottomRightSrcPoint);
    void setAllSrcPoints(cv::Point2f topLeftSrcPoint, cv::Point2f topRightSrcPoint,
           cv::Point2f bottomRightSrcPoint, cv::Point2f bottomLeftSrcPoint);
    void setAllSrcPoints(cv::Point2f srcPoints[4]);

    void setAllSrcPoints(const std::vector<cv::Point2f>& srcPoints);

    void setInputImage(const cv::Mat& inputImage);
    void doTheJob();

    void saveResultAsImage(std::string imagePath);
    void saveInputImage(std::string imagePath);
    cv::Mat getResult();

};