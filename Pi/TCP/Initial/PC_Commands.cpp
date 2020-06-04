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
#include <math.h>
#include <unistd.h>
#include <vector>

using namespace std;


// ImageProcessor IP;
const char vertical_sno [9] = "00295780";
uint8_t step_mode = 3;
uint8_t mult = (uint8_t) pow(2, (double)step_mode);
uint32_t max_speed = 2000000 * mult;
uint32_t starting_speed = 0;
uint32_t max_decel = 40000 * mult;
uint32_t max_accel = 40000 * mult; 
uint16_t current_limit = 1000;
int32_t curr_pos = 0;
int32_t full_vertical = 83;
TicDriver* vertical = createTicDriver(vertical_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

const vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}


string getCoordinate(){
    // vector<vector<float>> coord;
    // coord = IP.getCoordinate();
    return "2312.34567 89.0123456 0 0";
}

string gotoCenter(){
    return "04done";
}

string step(string command){
    vector<string> v{explode(command, ' ')};
    int steps = stoi(v[1]);

    if (v[2].compare("LR1") == 0){
        ;
    }
    else if (v[2].compare("LR2") == 0){
        ;
    }
    else if (v[2].compare("UD1") == 0){
        vertical->steps(vertical, steps);
    }
    else if (v[2].compare("UD2") == 0){
        ;
    }

    
    return "04done";
}

