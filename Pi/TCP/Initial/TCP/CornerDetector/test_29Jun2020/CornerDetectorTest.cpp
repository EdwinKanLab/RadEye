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
    Mat image1 = imread("../TestImages/testImage1.jpg", IMREAD_COLOR);
    Mat image2 = imread("../TestImages/testImage2.jpg", IMREAD_COLOR);
    Mat image3 = imread("../TestImages/testImage3.jpg", IMREAD_COLOR);
    Mat image4 = imread("../TestImages/testImage4.jpg", IMREAD_COLOR);
    Mat image5 = imread("../TestImages/testImage5.jpg", IMREAD_COLOR);
    if(!(image1.data && image2.data && image3.data && image4.data && image5.data))                              // Check for invalid input
    {
        cout <<  "Could not open or find one of the images" << std::endl ;
        return -1;
    }

    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now();;
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    auto total = duration.count();

    CornerDetector singleCorner(1,0.1,100);

    CornerDetector fourPlaneCorners(4, 0.1, 700);
    ///////////////////////////////////////////////////////////////////////////

    singleCorner.setInputImage(image1);

    start = chrono::high_resolution_clock::now();
    
    singleCorner.doTheJob();
    
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken to detect corner in testImage1.jpg: " << duration.count() << endl;
    total += duration.count();

    singleCorner.saveInputImage("../Results/1a_inputImage1.jpg");
    singleCorner.saveResultAsImage("../Results/1b_cornerImage1.jpg");

    ///////////////////////////////////////////////////////////////////////////

    singleCorner.setInputImage(image2);
    
    start = chrono::high_resolution_clock::now();
    
    singleCorner.doTheJob();
    
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken to detect corner in testImage2.jpg: " << duration.count() << endl;
    total += duration.count();

    singleCorner.saveInputImage("../Results/2a_inputImage2.jpg");
    singleCorner.saveResultAsImage("../Results/2b_cornerImage2.jpg");

    ///////////////////////////////////////////////////////////////////////////

    singleCorner.setInputImage(image3);
    
    start = chrono::high_resolution_clock::now();
    
    singleCorner.doTheJob();
    
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken to detect corner in testImage3.jpg: " << duration.count() << endl;
    total += duration.count();

    singleCorner.saveInputImage("../Results/3a_inputImage3.jpg");
    singleCorner.saveResultAsImage("../Results/3b_cornerImage3.jpg");

    ///////////////////////////////////////////////////////////////////////////

    singleCorner.setInputImage(image4);
    
    start = chrono::high_resolution_clock::now();
    
    singleCorner.doTheJob();
    
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken to detect corner in testImage4.jpg: " << duration.count() << endl;
    total += duration.count();
    cout << endl;
    cout << "Total time taken for the first 4 test images: " <<  total << endl;

    singleCorner.saveInputImage("../Results/4a_inputImage4a.jpg");
    singleCorner.saveResultAsImage("../Results/4b_cornerImage4b.jpg");

    ///////////////////////////////////////////////////////////////////////////

    fourPlaneCorners.setInputImage(image5);

    start = chrono::high_resolution_clock::now();
    
    fourPlaneCorners.doTheJob();
    
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken to detect corner in testImage5.jpg: " << duration.count() << endl;

    fourPlaneCorners.saveInputImage("../Results/5a_inputImage5a.jpg");
    fourPlaneCorners.saveResultAsImage("../Results/5b_cornerImage5b.jpg");

    ///////////////////////////////////////////////////////////////////////////
    cout << "Finished Test" << endl;
    cout << "Check Results directory for the image files" << endl;
}
