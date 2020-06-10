/*
 * TicDriver.h
 *
 * Created: 07-May-2020
 * Last Modified: 07-May-2020
 * Author : tejas_advait
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
typedef struct  TicDriver{
	
	char serial_no[9];

	uint32_t max_speed;
	uint32_t starting_speed;
	uint32_t max_decel;
	uint32_t max_accel;
	
	uint8_t step_mode;
	uint16_t current_limit;

	int32_t curr_pos;
	int32_t target_pos;

	uint8_t isEnergized;

	int (*energize)(struct TicDriver*);

	int (*deenergize)(struct TicDriver*);

	int (*setStepMode)(struct TicDriver*, uint8_t val);	/// Set step mode

	int (*setCurrentLimit)(struct TicDriver*, uint16_t val);

	int (*setTargetPos)(struct TicDriver*, int32_t val);

	int (*setCurrPos)(struct TicDriver*, int32_t val);

	int (*setMaxDecel)(struct TicDriver*, uint32_t val);

	int (*setMaxAccel)(struct TicDriver*, uint32_t val);

	int (*setStartingSpeed)(struct TicDriver*, uint32_t val);

	int (*setMaxSpeed)(struct TicDriver*, uint32_t val);

	int (*steps) (struct TicDriver*, int32_t val);

	int (*updateCurrPos)(struct TicDriver*);



} TicDriver;


TicDriver* createTicDriver(const char* serial_no, uint32_t max_speed, uint32_t starting_speed, uint32_t max_decel, uint32_t max_accel, uint8_t step_mode, uint16_t current_limit, int32_t curr_pos);

TicDriver* copyTicDriver(TicDriver const* in);

void freeTicDriver(TicDriver* in);