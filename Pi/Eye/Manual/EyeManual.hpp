#ifndef EYEMANUAL
#define EYEMANUAL

#include <stdint.h>
#include "PiGPIO.h"
#include "PiComm.h"

struct EyeManual{

	//Need a Constructor
	//Need a start function to enable manual control

	//rots for rotation of two motors, zeroth index for alpha (horizontal angle) and beta (vertical angle)
	//steps_per_rot = steps_per_rotation of the stepper motor
	//numFracs = number of microstep resolutions;
	//Fracs = microstep resolutions (higher to lower, example [16, 8, 4, 1])
	//setRots(angle_a, angle_b) to set rotations based on angle and highest resolution
	//comm = communication bus between Pi and uC
	//sendTouC() to send motor data to uC

	int16_t rots[2];
	uint8_t numFracs;
	uint8_t* Fracs;
	PiComm* comm;
	int16_t steps_per_rot;

	// EyeManual(int16_t steps_per_rot, uint8_t numFracs, uint8_t* Fracs, PiComm* comm );

	EyeManual(int16_t steps_per_rot, uint8_t numFracs, uint8_t* Fracs, PiComm* comm);
	void start();
	void setRots(float angle_a, float angle_b);
	void sendTouC();

};

#endif
