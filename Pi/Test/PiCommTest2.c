/*
 * PiCommTest2.c
 *
 * Created: 14-Feb-2020
 * Last Modified: 14-Feb-2020
 * Author : tejas_advait
 */

#include "PiGPIO.h"
#include "PiComm.h"
#include "misc.h"

int main(){

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

	PiGPIO* DATPORT[8] = {DP0, DP1, DP2, DP3, DP4, DP5, DP6, DP7};

	uint8_t numCycles = 255;

	uint8_t DATA[numCycles];

	uint8_t i;

	for (i = 0; i<numCycles; i++){
		DATA[i] = i+1;
	}

	PiComm* comm = createPiComm(ENPIN, DNPIN, DATPORT, numCycles, DATA);

	
	comm->writeCycle(comm);
	
	ENPIN->writeLow(ENPIN);
	
	return 0;
}
