#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 4)
    {
     cout <<" Usage: four_boxes ImageToLoadAndDisplay box_ratio_h box_ratio_w" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], IMREAD_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }

    //update crop values here
    float topl_crop [2] = {0.16, 0.1};
    float botr_crop [2] = {0.96, 0.97};

    int range[2][2];
    range[0][0] = (int) image.rows*topl_crop[0];
    range[0][1] = (int) image.rows*botr_crop[0];
    range[1][0] = (int) image.cols*topl_crop[1];
    range[1][1] = (int) image.cols*botr_crop[1];

    Mat croppedImg = image(Range(range[0][0],range[0][1]), Range(range[1][0], range[1][1]));


    //default values of box ratio
    float box_ratio[2] = {0.1, 0.1};

    box_ratio[0] = stof(argv[2]);
    box_ratio[1] = stof(argv[3]);

    int box_dim [2];
    box_dim[0] = (int) croppedImg.rows*box_ratio[0];
    box_dim[1] = (int) croppedImg.cols*box_ratio[1]; 

    Mat topl_box = croppedImg(Range(0, box_dim[0]), Range(0, box_dim[1]));
    Mat topr_box = croppedImg(Range(0,box_dim[0]), Range(croppedImg.cols-box_dim[1],croppedImg.cols));
    Mat botr_box = croppedImg(Range(croppedImg.rows-box_dim[0],croppedImg.rows), Range(croppedImg.cols-box_dim[1],croppedImg.cols));
    Mat botl_box = croppedImg(Range(croppedImg.rows-box_dim[0],croppedImg.rows), Range(0,box_dim[1]));


    // cout << "Topl number of rows " << topl_box.rows << endl;
    // cout << "box dim  " << box_dim[0] << endl;

    // namedWindow( "Original Image", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Original Image", image );                   // Show our image inside it.

    // namedWindow( "Cropped Image", WINDOW_AUTOSIZE); // Create a window for display.
    // imshow( "Cropped Image", croppedImg );                   // Show our image inside it.

    // namedWindow( "Top Left Box", WINDOW_NORMAL ); 
    // imshow( "Top Left Box", topl_box );    

    // namedWindow( "Top Right Box", WINDOW_NORMAL );
    // imshow("Top Right Box", topr_box);
    
    // namedWindow( "Bottom Right Box", WINDOW_NORMAL );
    // imshow("Bottom Right Box", botr_box);
    
    // namedWindow( "Bottom Left Box", WINDOW_NORMAL );
    // imshow("Bottom Left Box", botl_box);

    // waitKey(0);                                          // Wait for a keystroke in the window

    imwrite("croppedImg.jpeg", croppedImg);
    imwrite("topl_box.jpeg", topl_box);
    imwrite("topr_box.jpeg", topr_box);
    imwrite("botr_box.jpeg", botr_box);
    imwrite("botl_box.jpeg", botl_box);

    return 0;
}
