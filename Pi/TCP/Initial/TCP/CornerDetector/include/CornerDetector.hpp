#pragma once
#include <opencv2/core/mat.hpp>
#include <string>
#include <vector>

class CornerDetector{
private:
    cv::Mat inputImage;
 	std::vector<cv::Point2f> result;
	int	maxCorners;
	double qualityLevel;
	double minDistance;
	cv::Mat mask;
	int  blockSize;
	bool useHarrisDetector;
	double  k;

    cv::Mat grayImage;

    
    

public:
    CornerDetector();
    CornerDetector(int maxCorners, double qualityLevel, double minDistance);
    CornerDetector(int maxCorners, double qualityLevel, double minDistance, int blockSize);
    CornerDetector(int maxCorners, double qualityLevel,
                    double minDistance, cv::Mat mask, int blockSize,
                    bool useHarrisDetector, double k);
    
    void setInputImage(const cv::Mat& inputImage);
    void doTheJob();
    void saveResultAsImage(std::string imagePath);
    void saveInputImage(std::string imagePath);
    std::vector<cv::Point2f> getResult();

};