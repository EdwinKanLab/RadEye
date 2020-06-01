#include "CommBus.h"
#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000L
#endif



static uint8_t readEn(CommBus* self){ return (self->ENPIN)->read(self->ENPIN); }



static void readSingle(CommBus* self){
	self->setDnHigh(self);

	while(!self->readEn(self)) {;}
	_delay_ms(100);

	self->DATASINGLE = (self->DATPORT)->read(self->DATPORT);

	self->setDnLow(self);

	while(self->readEn(self)) {;}
	_delay_ms(100);
}



// static void readSingle(CommBus* self){

// 	self->setDnHigh(self);

// 	while(!self->readEn(self)) {;}

// 	uint8_t done = 0;
	
// 	uint8_t iter = 0;
	
// 	uint8_t r;
// 	uint8_t temp;
// 	uint8_t count;


// 	while (!done){
// 		temp = (self->DATPORT)->read(self->DATPORT);
// 		count = 0;
		
// 		while (temp != 0){
// 			temp = (temp) & (temp - 1);
// 			count++;
// 		}

// 		r = (self->DATPORT)->read(self->DATPORT);

// 		if ( (count % 2 == 0) && (r & 1) ) done = 1;

// 		if ((count % 2 == 1) && !(r & 1) ) done = 1;

// 		if (iter > 10) done = 1;

// 		iter ++;
// 	}

// 	if (iter > 10) self->DATASINGLE = 0x01;

// 	if (iter <= 10) self->DATASINGLE = r;
// };

static void readCycle(CommBus* self){ 
	// assert(self->isDat(self) == 1);

	uint8_t temp = self->numCycles;

	while (temp > 0){
		self->readSingle(self);
		self->DATA[self->numCycles - temp] = self->DATASINGLE;
		temp --;
	}
}

static void setDnHigh(CommBus* self){ (self->DNPIN)->writeHigh(self->DNPIN); }

static void setDnLow(CommBus* self){ (self->DNPIN)->writeLow(self->DNPIN); }






CommBus* createCommBus(GPIO* ENPIN, GPIO* DNPIN, GPIORT* DATPORT, uint8_t numCycles){
	CommBus* cbus = (CommBus*) malloc(sizeof(CommBus) + sizeof(uint8_t) * numCycles);

	if (cbus == NULL) {
	/* Handle malloc failure */
	;
	}

	*cbus = (CommBus){.ENPIN=ENPIN, .DNPIN=DNPIN,
						.DATPORT=DATPORT, .numCycles=numCycles,
						.setDnHigh=setDnHigh, .setDnLow=setDnLow,
						.readSingle=readSingle, .readCycle=readCycle,
						.readEn=readEn, .DATASINGLE=0};


	for (uint8_t i = 0; i < numCycles; i++){
		cbus->DATA[i] = 0;
	}

	(cbus->DNPIN)->setAsOutput(cbus->DNPIN);
	cbus->setDnLow(cbus);
	
	(cbus->ENPIN)->setAsInput(cbus->ENPIN);
	(cbus->DATPORT)->setInputs(cbus->DATPORT, 0xFF);

	return cbus;
}

CommBus* copyCommBus(CommBus const* in){
	CommBus* out = malloc(sizeof(CommBus) + (sizeof(uint8_t) * (in->numCycles) ));

	*out = *in;
	return out;
}

void freeCommBus(CommBus* in){ free(in); }
