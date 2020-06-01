#include "EyeManual.hpp"
#include "PiGPIO.h"
#include "PiComm.h"
#include "misc.h"

int main(){
	
	// Pins for communication
	PiGPIO* ENPIN = createPiGPIO(26);
	PiGPIO* DNPIN = createPiGPIO(19);

	PiGPIO* DP7 = createPiGPIO(21);
	PiGPIO* DP6 = createPiGPIO(20);
	PiGPIO* DP5 = createPiGPIO(16);
	PiGPIO* DP4 = createPiGPIO(12);
	PiGPIO* DP3 = createPiGPIO(1);
	PiGPIO* DP2 = createPiGPIO(7);
	PiGPIO* DP1 = createPiGPIO(8);
	PiGPIO* DP0 = createPiGPIO(25);

	//Dataport
	PiGPIO* DATPORT[8] = {DP0, DP1, DP2, DP3, DP4, DP5, DP6, DP7};

	//Number of cycles for communication
	int numCycles = 4;
	uint8_t DATA[numCycles];

	//PiComm object for communication
	PiComm* comm = createPiComm(ENPIN, DNPIN, DATPORT, numCycles, DATA);

	int16_t steps_per_rot = 400;
	uint8_t numFracs = 1;
	uint8_t Fracs[1] = {1};

	EyeManual eye1 = EyeManual(steps_per_rot, 1, Fracs, comm);
	//EyeManual eye1 (steps_per_rot, numFracs, Fracs);
	eye1.start();

}
