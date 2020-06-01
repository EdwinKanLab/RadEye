#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void dstPoints(Point2f* src, Point2f* dst)
{
    double top_width = norm(src[0] - src[1]);
    double bottom_width = norm(src[2] - src[3]);
    double left_height = norm(src[0] - src[3]);
    double right_height = norm(src[1] - src[2]);

    int max_height = (int) round(max(left_height, right_height));
    int max_width = (int) round(max(top_width, bottom_width));
    dst[0] = Point2f(0,0);
    dst[1] = Point2f(max_width,0);
    dst[2] = Point2f(max_width, max_height);
    dst[3] = Point2f(0, max_height);

}

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
    corners[2] = botr_corner[0] + Point2f(croppedImg.cols - box_dim[1] , croppedImg.rows - box_dim[0]);
    corners[3] = botl_corner[0] + Point2f(0, croppedImg.rows - box_dim[0]);




    // cout << "type "<< croppedImg.type() << endl;
    Point2f dstPts[4];

    dstPoints(corners, dstPts);

    Size warped_size(dstPts[2].x, dstPts[2].y);

    Mat warped(warped_size, croppedImg.type());

    Mat M = getPerspectiveTransform(corners,dstPts);
    warpPerspective(croppedImg, warped, M, warped_size);

    for (int i = 0; i < 4; i++){
        circle(croppedImg, corners[i], 3, 255, -1);
    }
    
    namedWindow( "Original Image", WINDOW_AUTOSIZE); // Create a window for display.
    imshow( "Original Image", image );                   // Show our image inside it.

    // namedWindow( "Grayscale Image", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Grayscale Image", gray );                   // Show our image inside it.


    // namedWindow( "Cropped Image With Corners", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Cropped Image With Corners", croppedImg );                   // Show our image inside it.

    // namedWindow( "Warped Image", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Warped Image", warped );

    // namedWindow( "Top Left Box", WINDOW_NORMAL ); 
    // imshow( "Top Left Box", topl_box );    

    // namedWindow( "Top Right Box", WINDOW_NORMAL );
    // imshow("Top Right Box", topr_box);
    
    // namedWindow( "Bottom Right Box", WINDOW_NORMAL );
    // imshow("Bottom Right Box", botr_box);
    
    // namedWindow( "Bottom Left Box", WINDOW_NORMAL );
    // imshow("Bottom Left Box", botl_box);

    // waitKey(0);                                          // Wait for a keystroke in the window
    imwrite("warped.jpeg", warped);
    imwrite("croppedWcorners.jpeg", croppedImg);
    return 0;
}
