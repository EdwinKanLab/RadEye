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
// using namespace cv;
class ImageProcessor{
private:
    vector<float> topl_crop(2,0);
public:
    vector<vector<float>> getCoordinate();
};
