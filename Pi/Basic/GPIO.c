/*
 * GPIO.c
 *
 * Created: 17-Jan-20 10:00:12 AM
 * Author : ta275
 */ 

#include "GPIO.h"
#include <stdlib.h>

static void setAsOutput(GPIO* self){ *(self->DDR) |= (1<<self->PINNUM); } 					/// Set the corresponding pin in DDR as 1 for output

static void setAsInput(GPIO* self){
	*(self->DDR) &= ~(1<<self->PINNUM);
	*(self->PORT) |= (1<<self->PINNUM);
}

static void writeHigh(GPIO* self){ *(self->PORT) |= (1<<self->PINNUM) & *(self->DDR); }	/// Set the corresponding pin in PORT as 1 for high

static void writeLow(GPIO* self){ *(self->PORT) &= ~((1<<self->PINNUM) & *(self->DDR)); }	/// Set the corresponding pin in PORT as 0 for low

static uint8_t read(GPIO* self){ 

	/// Return the value of the corresponding pin in PORT if output, PIN if input
	return (((((*(self->DDR)) & (*(self->PORT))) | ((~(*(self->DDR))) & (*(self->PIN)))) >> self->PINNUM) & 0x01);
}

// static uint8_t read(GPIO* self){ return ( (*(self->PORT)) & 0b00000001); }

GPIO* createGPIO(volatile uint8_t* DDR, volatile uint8_t* PORT, volatile uint8_t* PIN, uint8_t PINNUM){
	GPIO* gpio = (GPIO*) malloc(sizeof(GPIO));
	if (gpio == NULL){
		/*Handle malloc failure*/
		;
	}

	*gpio = (GPIO){.DDR=DDR, .PORT=PORT, .PIN=PIN, .PINNUM=PINNUM,.setAsOutput=setAsOutput,
		.setAsInput=setAsInput, .writeHigh=writeHigh, .writeLow=writeLow, .read=read};

	return gpio;
}

GPIO* copyGPIO(GPIO const* in){
	GPIO* out = malloc(sizeof(GPIO));
	*out = *in;
	return out;
}

void freeGPIO(GPIO* in){ free(in); }