#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: four_corners ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], IMREAD_COLOR);   // Read the file
    // image = imread(argv[1], IMREAD_GRAYSCALE); 


    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }

    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    //update crop values here
    float topl_crop [2] = {0.16, 0.1};
    float botr_crop [2] = {0.96, 0.97};

    int range[2][2];
    range[0][0] = (int) image.rows*topl_crop[0];
    range[0][1] = (int) image.rows*botr_crop[0];
    range[1][0] = (int) image.cols*topl_crop[1];
    range[1][1] = (int) image.cols*botr_crop[1];

    Mat croppedImg = gray(Range(range[0][0],range[0][1]), Range(range[1][0], range[1][1]));


    //default values of box ratio
    float box_ratio[2] = {0.1, 0.1};

    int box_dim [2];
    box_dim[0] = (int) croppedImg.rows*box_ratio[0];
    box_dim[1] = (int) croppedImg.cols*box_ratio[1]; 

    Mat topl_box = croppedImg(Range(0, box_dim[0]), Range(0, box_dim[1]));
    Mat topr_box = croppedImg(Range(0,box_dim[0]), Range(croppedImg.cols-box_dim[1],croppedImg.cols));
    Mat botr_box = croppedImg(Range(croppedImg.rows-box_dim[0],croppedImg.rows), Range(croppedImg.cols-box_dim[1],croppedImg.cols));
    Mat botl_box = croppedImg(Range(croppedImg.rows-box_dim[0],croppedImg.rows), Range(0,box_dim[1]));


    // vector< Point2f > corners;
    // Point2f topl_corner [1];
    vector<Point2f> topl_corner;
    vector<Point2f> topr_corner;
    vector<Point2f> botr_corner;
    vector<Point2f> botl_corner;
    goodFeaturesToTrack(topl_box, topl_corner, 1, 0.1, 100);
    goodFeaturesToTrack(topr_box, topr_corner, 1, 0.1, 100);
    goodFeaturesToTrack(botr_box, botr_corner, 1, 0.1, 100);
    goodFeaturesToTrack(botl_box, botl_corner, 1, 0.1, 100);
    Point2f corners[4];
    corners[0] = topl_corner[0];
    corners[1] = topr_corner[0] + Point2f(croppedImg.cols - box_dim[1] , 0);
    // corners[1] = topr_corner[1];
    corners[2] = botr_corner[0] + Point2f(croppedImg.cols - box_dim[1] , croppedImg.rows - box_dim[0]);
    corners[3] = botl_corner[0] + Point2f(0, croppedImg.rows - box_dim[0]);


    // corners.insert(topl_corner[0]);

    // cout << "point1 "<< topl_corner[0] << endl;
    // cout << "point2 "<< topr_corner[0] << endl;
    // cout << "point1+[0,5] "<< topl_corner[0]+Point2f(0,5) << endl; 
    // corners.insert(topr_corner[0])
    // cout << "corner " << topl_corner << endl;
    // cout << "corner size " << topl_corner.size() << endl;
    // cout << "corner x " << topl_corner[0].x << endl;
    // cout << "botr_corner "<< botr_corner[0] << endl;
    // circle(topl_box, topl_corner[0], 3, 255, -1);
    // circle(topr_box, topr_corner[0], 3, 255, -1);
    // circle(botr_box, botr_corner[0], 3, 255, -1);

    // circle(croppedImg, corners[0], 3, 255, -1);
    // circle(croppedImg, corners[1], 3, 255, -1);
    for (int i = 0; i < 4; i++){
        circle(croppedImg, corners[i], 3, 255, -1);
    }
    

    // cout << "Topl number of rows " << topl_box.rows << endl;
    // cout << "box dim  " << box_dim[0] << endl;

    // namedWindow( "Original Image", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Original Image", image );                   // Show our image inside it.

    // namedWindow( "Grayscale Image", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Grayscale Image", gray );                   // Show our image inside it.

    // namedWindow( "Cropped Image With Corners", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Cropped Image With Corners", croppedImg );                   // Show our image inside it.

    // namedWindow( "Top Left Box", WINDOW_NORMAL ); 
    // imshow( "Top Left Box", topl_box );    

    // namedWindow( "Top Right Box", WINDOW_NORMAL );
    // imshow("Top Right Box", topr_box);
    
    // namedWindow( "Bottom Right Box", WINDOW_NORMAL );
    // imshow("Bottom Right Box", botr_box);
    
    // namedWindow( "Bottom Left Box", WINDOW_NORMAL );
    // imshow("Bottom Left Box", botl_box);

    // waitKey(0);                                          // Wait for a keystroke in the window
    imwrite("croppedWcorners.jpeg", croppedImg);
    return 0;
}
