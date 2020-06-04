/*
 *PC_commands.c
 *
 * Created: 03-Jun-20
 * Last Modified: 03-Jun-2020
 * Author: tejas_advait
 *
 * Description: Commands received from PC to be executed on Pi. 
 */ 

#include "PC_Commands.hpp"
#include "TicDriver.h"
// #include <ImageProcessor.hpp>
#include <string>

using namespace std;


// ImageProcessor IP;


string getCoordinate(){
    // vector<vector<float>> coord;
    // coord = IP.getCoordinate();
    return "2212.34567 89.0123456 0 0";
}

string gotoCenter(){
    return "04done";
}

string step(string command){
    return "04done";
}