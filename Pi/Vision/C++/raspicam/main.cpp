#include <ctime>
#include <iostream>
#include <raspicam/raspicam_cv.h>
using namespace std; 
using namespace cv; 
int main ( int argc,char **argv ) {
   
    time_t timer_begin,timer_end;
    raspicam::RaspiCam_Cv Camera;
    
    int nCount=100;
    cv::Mat image[nCount];
    //set camera params
    Camera.set( CAP_PROP_FORMAT, CV_32FC3 );
    //Open camera
    cout<<"Opening Camera..."<<endl;
    if (!Camera.open()) {cerr<<"Error opening the camera"<<endl;return -1;}
    //Start capture
    cout<<"Capturing "<<nCount<<" frames ...."<<endl;
    time ( &timer_begin );
    for ( int i=0; i<nCount; i++ ) {
        Camera.grab();
        Camera.retrieve ( image[i]);
        if ( i%5==0 )  cout<<"\r captured "<<i<<" images"<<std::flush;
    }
    cout<<"Stop camera..."<<endl;
    Camera.release();
    //show time statistics
    time ( &timer_end ); /* get current time; same as: timer = time(NULL)  */
    double secondsElapsed = difftime ( timer_end,timer_begin );
    cout<< secondsElapsed<<" seconds for "<< nCount<<"  frames : FPS = "<<  ( float ) ( ( float ) ( nCount ) /secondsElapsed ) <<endl;
    //save image
    for (int i = 0; i < nCount; i++){
        cv::imwrite("images/raspicam_cv_image_" + to_string(i) + ".jpg",image[i]);
    }
    cout<<"All images saved at raspicam_cv_image.jpg"<<endl;
}
