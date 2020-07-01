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
    Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
    Cropper topLeftRegion(0, 0.10, 0.01, 0.10);
    Cropper topRightRegion(0.05, 0.15, 0.85, 1);
    Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
    Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);

    for (int i = 1; i <= 5; i++){

        Mat image = imread("../TestImages/testImage"+ to_string(i) + ".jpg", IMREAD_COLOR);
        if(! image.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }
        // cout << "line 16" << endl;

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

        initialDiscard.saveInputImage("../Results/"+to_string(i)+"a_inputImage.jpg");
        initialDiscard.saveResultAsImage("../Results/"+to_string(i)+"b_initialDiscard.jpg");
        topLeftRegion.saveResultAsImage("../Results/"+to_string(i)+"c_topLeftRegion.jpg");
        topRightRegion.saveResultAsImage("../Results/"+to_string(i)+"d_topRightRegion.jpg");
        bottomLeftRegion.saveResultAsImage("../Results/"+to_string(i)+"e_bottomLeftRegion.jpg");
        bottomRightRegion.saveResultAsImage("../Results/"+to_string(i)+"f_bottomRightRegion.jpg");
    }
    cout << "Finished Test" << endl;
    cout << "Check Results directory for the image files" << endl;


}