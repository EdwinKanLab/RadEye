#include "../include/Cropper.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <filesystem>
#include <string>

using namespace cv;
using namespace std;
// using path = std::filesystem::path;

int main(){
    Mat image = imread("../testImage.jpg", IMREAD_COLOR);
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    // cout << "line 16" << endl;
    Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
    Cropper topLeftRegion(0, 0.10, 0.0, 0.10);
    Cropper topRightRegion(0.05, 0.15, 0.85, 1);
    Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
    Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);
    // cout << "line 22" << endl;

    initialDiscard.setInputImage(image);
    // cout << "line 25" << endl;
    initialDiscard.doTheJob();
    // cout << "line 27" << endl;

    topLeftRegion.setInputImage(initialDiscard.getResult());
    topRightRegion.setInputImage(initialDiscard.getResult());
    bottomLeftRegion.setInputImage(initialDiscard.getResult());
    bottomRightRegion.setInputImage(initialDiscard.getResult());
    topLeftRegion.doTheJob();
    topRightRegion.doTheJob();
    bottomLeftRegion.doTheJob();
    bottomRightRegion.doTheJob();

    initialDiscard.saveInputImage("../Results/1_inputImage.jpg");
    initialDiscard.saveResultAsImage("../Results/2_initialDiscard.jpg");
    topLeftRegion.saveResultAsImage("../Results/3_topLeftRegion.jpg");
    topRightRegion.saveResultAsImage("../Results/4_topRightRegion.jpg");
    bottomLeftRegion.saveResultAsImage("../Results/5_bottomLeftRegion.jpg");
    bottomRightRegion.saveResultAsImage("../Results/6_bottomRightRegion.jpg");
    cout << "Finished Test" << endl;
    cout << "Check Results directory for the image files" << endl;


}