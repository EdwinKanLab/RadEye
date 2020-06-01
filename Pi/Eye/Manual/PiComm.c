/*
 * PiComm.c
 *
 * Created: 14-Feb-2020
 * Last Modified: 23-Feb-2020
 * Author : tejas_advait
 *
 * Description: Source code for PiComm.h.
 */

#include "PiComm.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

static uint8_t readDn(PiComm* self){ return (self->DNPIN)->read(self->DNPIN); }


static void writeSingle(PiComm* self){
	
	self->setEnLow(self);

	while(!self->readDn(self)) {;}

	uint8_t i;
	uint8_t bit;

	for (i = 0; i < 8; i++){
		bit = ((self->DATASINGLE) >> i) & 0x01;

		if (bit){ (self->DATPORT[i])->writeHigh(self->DATPORT[i]); }

		if (!bit){ (self->DATPORT[i])->writeLow(self->DATPORT[i]); }

	}

	self->setEnHigh(self);

	while (self->readDn(self)) {;}
	
}



static void writeCycle(PiComm* self){ 

	uint8_t i;

	for (i = 0; i < self->numCycles; i++){
		self->DATASINGLE = self->DATA[i];
		self->writeSingle(self);
	}

}

static void setEnHigh(PiComm* self){ (self->ENPIN)->writeHigh(self->ENPIN); }

static void setEnLow(PiComm* self){ (self->ENPIN)->writeLow(self->ENPIN); }

static void setData(PiComm* self, int16_t* rots){
	uint8_t left;
	uint8_t right;
	
	uint8_t i;
	
	for (i = 0; i < (self->numCycles)/2; i++){

		left = (uint8_t) ((rots[i] >> 8) & 0x00FF);
		right = (uint8_t) (rots[i] & 0x00FF);
		self->DATA[2*i] = left;
		self->DATA[(2*i)+1] = right;
	}
}

PiComm* createPiComm(PiGPIO* ENPIN, PiGPIO* DNPIN, PiGPIO** DATPORT, uint8_t numCycles, uint8_t* DATA){
	PiComm* cbus = (PiComm*) malloc(sizeof(PiComm));

	if (cbus == NULL) {
	/* Handle malloc failure */
	;
	}

	*cbus = (PiComm){ .ENPIN=ENPIN, .DNPIN=DNPIN,
		.DATPORT=DATPORT, .numCycles=numCycles, .DATASINGLE=0,
		.DATA=DATA, .readDn=readDn,
		 .writeSingle=writeSingle, .writeCycle=writeCycle,
		.setEnHigh=setEnHigh, .setEnLow=setEnLow, .setData=setData,
		 };

	(cbus->DNPIN)->setAsInput(cbus->DNPIN);
	
	
	(cbus->ENPIN)->setAsOutput(cbus->ENPIN);
	cbus->setEnLow(cbus);

	uint8_t i;
	for (i = 0; i < 8; i++){
		(cbus->DATPORT[i])->setAsOutput(cbus->DATPORT[i]);
	}

	return cbus;
}

PiComm* copyPiComm(PiComm const* in){
	PiComm* out = (PiComm*) malloc(sizeof(PiComm));

	*out = *in;
	return out;
}

void freePiComm(PiComm* in){ free(in); }
