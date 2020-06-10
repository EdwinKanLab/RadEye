#pragma once
#include <opencv2/core/mat.hpp>
#include <string>

class Cropper{
private:
    float topRatio;
    float leftRatio;
    float bottomRatio;
    float rightRatio;

    cv::Range height;
    cv::Range width;

    cv::Mat inputImage;
    cv::Mat result;

    void updateHeightAndWidth();
public:
    Cropper();
    Cropper(float topRatio, float bottomRatio, float leftRatio,  float rightRatio);
    void setTopRatio(float topRatio);
    void setBottomRatio(float bottomRatio);
    void setLeftRatio(float leftRatio);
    void setRightRatio(float rightRatio);
    void setAllRatio(float topRatio, float bottomRatio, float leftRatio, float rightRatio);
    void setInputImage(const cv::Mat& inputImage);
    void doTheJob();
    void saveResultAsImage(std::string imagePath);
    void saveInputImage(std::string imagePath);
    float getTopRatio();
    float getBottomRatio();
    float getLeftRatio();
    float getRightRatio();
    cv::Mat getInputImage();
    cv::Mat getResult();

};