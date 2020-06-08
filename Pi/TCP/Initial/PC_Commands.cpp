/*
 *PC_commands.c
 *
 * Created: 03-Jun-20
 * Last Modified: 04-Jun-2020
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

const char vertical_1_sno [9] = "00295780";
const char horizontal_1_sno [9] = "00286960";

const char vertical_2_sno [9] = "00295780";
const char horizontal_2_sno [9] = "00286960";


uint8_t step_mode = 3;
uint8_t mult = (uint8_t) pow(2, (double)step_mode);
uint32_t max_speed = 2000000 * mult;
uint32_t starting_speed = 0;
uint32_t max_decel = 40000 * mult;
uint32_t max_accel = 40000 * mult; 
uint16_t current_limit = 1000;
int32_t curr_pos = 0;
int32_t full_vertical = 83;

TicDriver* vertical_1 = createTicDriver(vertical_1_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

TicDriver* horizontal_1 = createTicDriver(horizontal_1_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

TicDriver* vertical_2 = createTicDriver(vertical_2_sno, max_speed, starting_speed,
max_decel, max_accel, step_mode, current_limit, curr_pos);

TicDriver* horizontal_2 = createTicDriver(horizontal_2_sno, max_speed, starting_speed,
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
    return "23000.0 000.0 000.0 000.0";
}

string gotoCenter(){
    return "04done";
}

string step(string command){
    vector<string> v{explode(command, ' ')};
    int steps = stoi(v[1]);

    if (v[2].compare("LR1") == 0){
        if (steps != 0) horizontal_1->steps(horizontal_1, steps);
        else {horizontal_1->deenergize(horizontal_1);}
    }
    else if (v[2].compare("LR2") == 0){
        if (steps != 0) horizontal_2->steps(horizontal_2, steps);
        else {horizontal_2->deenergize(horizontal_2);}
    }
    else if (v[2].compare("UD1") == 0){
        if (steps != 0) vertical_1->steps(vertical_1, steps);
        else {vertical_1->deenergize(vertical_1);}
    }
    else if (v[2].compare("UD2") == 0){
        if (steps != 0) vertical_2->steps(vertical_2, steps);
        else {vertical_2->deenergize(vertical_2);}
    }
    
    return "04done";
}

