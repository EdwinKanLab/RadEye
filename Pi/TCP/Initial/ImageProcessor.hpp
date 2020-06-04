/*
 *ImageProcess.hpp
 *
 * Created: 03-Jun-20
 * Last Modified: 03-Jun-2020
 * Author: tejas_advait
 *
 * Description: Image processing on Pi. 
 */ 

#include <vector>
using namespace std;
using namespace cv;
class ImageProcessor{
private:
    float topl_crop[2];
    float botr_crop[2];
public:
    vector<vector<float>> getCoordinate();
};
