/*
 * PiGPIO.h
 *
 * Created: 12-Feb-20
 * Last Modified: 23-Feb-2020
 * Author: tejas_advait
 *
 * Description: Mini library for manipulating GPIO pins on the Raspberry Pi.
 */ 

#pragma once
#include <stdint.h>


/*
 * Struct: PiGPIO 
 * --------------------
 * Provides an abstraction for GPIO pins on the Raspberry Pi.
 *
 * Fields: PINNUM [uint32_t] - Pin number of the GPIO pin as per the pinout diagram.
 *		   
 *		   setupGPIO [volatile uint32_t*] - Required for DMA setup.
 *
 * Methods: setAsOutput [PiGPIO* -> void] - Set GPIO pin as output.
 *
 *			setAsInput	[PiGPIO* -> void] - Set GPIO pin as input.
 *
 *			writeHigh	[PiGPIO* -> void] - Write high to the GPIO pin.
 *
 *			writeLow	[PiGPIO* -> void] - Write low to the GPIO pin.
 *
 *			read 		[PiGPIO* -> uint8_t] - Read the GPIO pin value.
 */
typedef struct PiGPIO{

	uint32_t PINNUM;

	volatile uint32_t* setupGPIO;

	void (*setAsOutput)(struct PiGPIO*);

	void (*setAsInput)(struct PiGPIO*);

	void (*writeHigh)(struct PiGPIO*);

	void (*writeLow)(struct PiGPIO*);

	uint8_t (*read)(struct PiGPIO*);

} PiGPIO;


/*
 * Function:  createPiGPIO [uint32_t -> PiGPIO*]
 * --------------------
 * Creates a PiGPIO* from the pin number.
 *
 * PINNUM: Pin number of the GPIO pin as per the pinout diagram. [uint32_t]
 *
 * returns: Pointer to a PiGPIO struct [PiGPIO*].
 */
PiGPIO* createPiGPIO(uint32_t PINNUM);


/*
 * Function:  copyPiGPIO [PiGPIO const* -> PiGPIO*]
 * --------------------
 * Creates a deep copy of the input PiGPIO*.
 *
 * in: Input. [PiGPIO const*]
 *
 * returns: Pointer to a PiGPIO struct [PiGPIO*].
 */
PiGPIO* copyPiGPIO(PiGPIO const* in);

/*
 * Function:  freePiGPIO [PiGPIO* -> void]
 * --------------------
 * Frees the memory associated with the input PiGPIO*.
 *
 * in: Input. [PiGPIO*]
 *
 * returns: void.
 */
void freePiGPIO (PiGPIO* in);