#include "../../Cropper/include/Cropper.hpp"
#include "../include/CornerDetector.hpp"

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
// #include <filesystem>
#include <string>
#include <chrono> 

using namespace cv;
using namespace std;
// using path = std::filesystem::path;

int main(){
    Cropper initialDiscard(0.18, 0.98, 0.12, 0.97);
    Cropper topLeftRegion(0, 0.10, 0.01, 0.10);
    Cropper topRightRegion(0.05, 0.15, 0.85, 1);
    Cropper bottomLeftRegion(0.85, 0.98, 0, 0.15);
    Cropper bottomRightRegion(0.85, 1, 0.85, 0.99);
    
    CornerDetector singleCorner(1,0.1,100);
    CornerDetector fourPlaneCorners(4, 0.1, 700);

    for (int i = 1; i <= 5; i++){

        Mat image = imread("../TestImages/testImage"+ to_string(i) + ".jpg", IMREAD_COLOR);
        if(! image.data )                              // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }

        auto start = chrono::high_resolution_clock::now();
        auto stop = chrono::high_resolution_clock::now();;
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        auto total = duration.count();


        ///////////////////////////////////////////////////////////////////////////
        //Cropping
        initialDiscard.setInputImage(image);
        initialDiscard.doTheJob();

        topLeftRegion.setInputImage(initialDiscard.getResult());
        topRightRegion.setInputImage(initialDiscard.getResult());
        bottomLeftRegion.setInputImage(initialDiscard.getResult());
        bottomRightRegion.setInputImage(initialDiscard.getResult());
        topLeftRegion.doTheJob();
        topRightRegion.doTheJob();
        bottomLeftRegion.doTheJob();
        bottomRightRegion.doTheJob();

        ///////////////////////////////////////////////////////////////////////////
        singleCorner.setInputImage(topLeftRegion.getResult());

        start = chrono::high_resolution_clock::now();
        singleCorner.doTheJob();
        stop = chrono::high_resolution_clock::now();

        duration = chrono::duration_cast<chrono::microseconds>(stop - start);

        total = duration.count();

        singleCorner.saveInputImage("../Results/"+to_string(i)+"a_topLeftInputImage.jpg");
        singleCorner.saveGrayImage("../Results/"+to_string(i)+"a_topLeftGrayImage.jpg");
        singleCorner.saveResultAsImage("../Results/"+to_string(i)+"b_topLeftCornerImage.jpg");

        ///////////////////////////////////////////////////////////////////////////

        singleCorner.setInputImage(topRightRegion.getResult());
        
        start = chrono::high_resolution_clock::now();
        singleCorner.doTheJob();
        stop = chrono::high_resolution_clock::now();

        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
       
        total += duration.count();

        singleCorner.saveInputImage("../Results/"+to_string(i)+"c_topRightInputImage.jpg");
        singleCorner.saveGrayImage("../Results/"+to_string(i)+"c_topRightGrayImage.jpg");
        singleCorner.saveResultAsImage("../Results/"+to_string(i)+"d_topRightCornerImage.jpg");

        ///////////////////////////////////////////////////////////////////////////

        singleCorner.setInputImage(bottomRightRegion.getResult());
        
        start = chrono::high_resolution_clock::now();
        singleCorner.doTheJob();
        stop = chrono::high_resolution_clock::now();

        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        total += duration.count();

        singleCorner.saveInputImage("../Results/"+to_string(i)+"e_bottomRightInputImage.jpg");
        singleCorner.saveGrayImage("../Results/"+to_string(i)+"e_bottomRightGrayImage.jpg");
        singleCorner.saveResultAsImage("../Results/"+to_string(i)+"f_bottomRightCornerImage.jpg");

        ///////////////////////////////////////////////////////////////////////////

        singleCorner.setInputImage(bottomLeftRegion.getResult());
        
        start = chrono::high_resolution_clock::now();
        singleCorner.doTheJob();
        stop = chrono::high_resolution_clock::now();

        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        total += duration.count();

        singleCorner.saveInputImage("../Results/"+to_string(i)+"g_bottomLeftInputImage.jpg");
        singleCorner.saveGrayImage("../Results/"+to_string(i)+"g_bottomLeftGrayImage.jpg");
        singleCorner.saveResultAsImage("../Results/"+to_string(i)+"h_bottomLeftCornerImage.jpg");

        cout << "Time taken to detect corner in cropped images: " << total << endl;
        ///////////////////////////////////////////////////////////////////////////

        // fourPlaneCorners.setInputImage(initialDiscard.getResult());

        // start = chrono::high_resolution_clock::now();
        
        // fourPlaneCorners.doTheJob();
        
        // stop = chrono::high_resolution_clock::now();
        // duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        // cout << "Time taken to detect corner in initial discard: " << duration.count() << endl;

        // fourPlaneCorners.saveInputImage("../Results/"+to_string(i)+"i_initialDiscardInputImage.jpg");
        // fourPlaneCorners.saveResultAsImage("../Results/"+to_string(i)+"j_initialDiscardCornerImage.jpg");
    }
    ///////////////////////////////////////////////////////////////////////////
    cout << "Finished Test" << endl;
    cout << "Check Results directory for the image files" << endl;
}
