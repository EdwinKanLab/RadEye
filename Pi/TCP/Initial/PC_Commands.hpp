/*
 *PC_commands.hpp
 *
 * Created: 03-Jun-20
 * Last Modified: 03-Jun-2020
 * Author: tejas_advait
 *
 * Description: Commands received from PC to be executed on Pi. 
 */ 

#pragma once
#include <string>

using namespace std;

string getCoordinate();

string gotoCenter();

string step(string command);