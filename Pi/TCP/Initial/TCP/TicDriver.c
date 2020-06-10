/*
 * TicDriver.h
 *
 * Created: 07-May-2020
 * Last Modified: 07-May-2020
 * Author : tejas_advait
 */


#include "TicDriver.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
// #include <tic.h>


// Runs the given shell command.  Returns 0 on success, -1 on failure.
static int run_command(const char * command)
{
  int result = system(command);
  if (result)
  {
    fprintf(stderr, "Command failed with code %d: %s\n", result, command);
    return -1;
  }
  return 0;
}
static int energize (TicDriver* self)
{
	self->isEnergized = 1;
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --energize", self->serial_no);
	return run_command(command);
}

static int deenergize(TicDriver* self)
{
	self->isEnergized = 0;
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --deenergize", self->serial_no);
	return run_command(command);	
}
static int setStepMode(TicDriver* self, uint8_t val)
{
	self->step_mode = val;
	char command[1024];
	char mode[7];

	switch (val)
	{
		case 0: //Full-Step
  			// snprintf(command, sizeof(command), "ticcmd --step-mode full");
			snprintf(mode, sizeof(mode), "full");
			break;

		case 1:
			snprintf(mode, sizeof(mode), "half");
			break;

		case 2:
			snprintf(mode, sizeof(mode), "4");
			break;

		case 3:
			snprintf(mode, sizeof(mode), "8");
			break;

		case 4:
			snprintf(mode, sizeof(mode), "16");
			break;

		case 5:
			snprintf(mode, sizeof(mode), "32");
			break;

		case 6:
			snprintf(mode, sizeof(mode), "2_100p");
			break;

		default: 
			printf("Invalid step mode");
			return -1;
			break;
	}
	snprintf(command, sizeof(command), "ticcmd -d %s --step-mode %s", self->serial_no, mode);
	return run_command(command);
}

static int setCurrentLimit(TicDriver* self, uint16_t val)
{
	self->current_limit = val;
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --current %d", self->serial_no, val);
	return run_command(command);
}

static int setTargetPos(TicDriver* self, int32_t val)
{
	self->target_pos = val;
	if (!self->isEnergized) { self->energize(self); }
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd --exit-safe-start -d %s --position %d", self->serial_no, val);
	// int result = run_command(command);
	// if (result == 0)
	// {
	// 	self->curr_pos = val;
	// }
	// return result;
	return run_command(command);
}

static int setCurrPos(TicDriver* self, int32_t val)
{
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --halt-and-set-position %d  ", self->serial_no, val);
	return run_command(command);
}


static int setMaxDecel(TicDriver* self, uint32_t val)
{
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --max-decel %d  ", self->serial_no, val);
	return run_command(command);
}

static int setMaxAccel(TicDriver* self, uint32_t val)
{
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --max-accel %d  ", self->serial_no, val);
	return run_command(command);
}

static int setStartingSpeed(TicDriver* self, uint32_t val)
{
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --starting-speed %d  ", self->serial_no, val);
	return run_command(command);
}

static int setMaxSpeed(TicDriver* self, uint32_t val)
{
	char command[1024];
	snprintf(command, sizeof(command), "ticcmd -d %s --max-speed %d  ", self->serial_no, val);
	return run_command(command);
}


// static int step (TicDriver* self, int32_t dir,unsigned int delay)
// {
// 	int result = self->setTargetPos(self, self->curr_pos + dir);
// 	delayMicrosecondsHard(delay);
// 	return result;
// }
// static int steps(TicDriver* self, int32_t val){

// 	int result;
// 	if (val >= 0 )
// 	{
// 		for (int i = 0; i < val; i++)
// 		{
// 			result += step(self, 1, 1);
// 		}
// 	}

// 	if (val < 0)
// 	{
// 		for (int i = 0; i < (val*-1); i++)
// 		{
// 			result += step(self,-1,1);
// 		}
// 	}
// 	// return self->setTargetPos(self, self->curr_pos + val);
// 	if (result != 0) {return -1;}
// 	return 0;
// }
static int steps (TicDriver* self, int32_t val)
{
	int result = self->setTargetPos(self, self->curr_pos + val);
	self->updateCurrPos(self);
	while (self->curr_pos != self->target_pos)
	{
		self->updateCurrPos(self);
		//printf ("%d \n", self->curr_pos);
	}
	return result;
}

static int updateCurrPos(TicDriver* self)
{
	FILE* fp;
	char str1[21];
	char str2[21];

	int result = 1;
	int32_t pos;


	char command[201];
	snprintf(command, sizeof(command), "ticcmd -d %s -s", self->serial_no);

	fp = popen(command, "r");
	if (fp == NULL){return -1;}
	while (result != EOF){
		result = fscanf(fp, "%s", str1);

		if (strcmp(str1, "Current") == 0)
		{
			fscanf(fp, "%s", str2);
			if (strcmp(str2, "position:") == 0)
			{
				fscanf(fp, "%d", &pos);
				// printf("current position is %d\n", pos);
				pclose(fp);
				result = EOF;
			}
		}
		
	}
	self->curr_pos = pos;
	return 0;

}


TicDriver* createTicDriver(const char* serial_no, uint32_t max_speed, 
	uint32_t starting_speed, uint32_t max_decel, uint32_t max_accel, 
	uint8_t step_mode, uint16_t current_limit, int32_t curr_pos)
{
	TicDriver* driver = (TicDriver*) malloc(sizeof(TicDriver));


	if (driver == NULL) {
	/* Handle malloc failure */
	;
	}

	*driver = (TicDriver){
						  .max_speed=max_speed, .starting_speed=starting_speed, 
						  .max_decel=max_decel, .max_accel=max_accel,
						  .step_mode=step_mode, .current_limit=current_limit,
						  .curr_pos=curr_pos, .isEnergized=0,
						  .energize=energize, .deenergize=deenergize,
						  .setStepMode=setStepMode, .setCurrentLimit=setCurrentLimit,
						  .setTargetPos=setTargetPos, .setCurrPos=setCurrPos,
						  .setMaxDecel=setMaxDecel, .setMaxAccel=setMaxAccel,
						  .setStartingSpeed=setStartingSpeed,
						  .setMaxSpeed=setMaxSpeed, .steps=steps, .updateCurrPos=updateCurrPos
						};

	snprintf(driver->serial_no, sizeof(driver->serial_no), serial_no);
	
	///////////////////////////
	driver->deenergize(driver);
	///////////////////////////
	
	driver->setMaxSpeed(driver,max_speed);
	driver->setStartingSpeed(driver,starting_speed);
	driver->setMaxDecel(driver, max_decel);
	driver->setMaxAccel(driver, max_accel);
	driver->setStepMode(driver, step_mode);
	driver->setCurrentLimit(driver, current_limit);
	driver->setCurrPos(driver, curr_pos);

	return driver;
}

TicDriver* copyTicDriver(TicDriver const* in){
	TicDriver* out = (TicDriver*) malloc(sizeof(TicDriver));
	*out = *in;
	return out;
}

void freeTicDriver(TicDriver* in){ free(in); }
