/*
 * GPIO.h
 *
 * Created: 17-Jan-20 10:00:12 AM
 * Author : ta275
 *
 * Description: Use the GPIO type and associated methods for manipulating GPIO pins.
 */ 
#pragma once
#include <stdint.h>
typedef struct GPIO{
	volatile uint8_t* DDR;	/// Data Direction Register of the GPIO pin
	volatile uint8_t* PORT; /// Port Register of the GPIO pin
	volatile uint8_t* PIN;  /// Pin Register of the GPIO pin
	uint8_t PINNUM;			/// Pin number (Port Register/Pin bit number) of the GPIO pin

	void (*setAsOutput)(struct GPIO*);	/// Set GPIO pin as output

	void (*setAsInput)(struct GPIO*);	/// Set GPIO pin as input

	void (*writeHigh)(struct GPIO*);	/// Write high to the GPIO pin (GPIO pin must be declared as output before this method is called)

	void (*writeLow)(struct GPIO*);		/// Write low to the GPIO pin (GPIO pin must be declared as output before this method is called)

	uint8_t (*read)(struct GPIO*);		/// Read the GPIO pin value

} GPIO;


GPIO* createGPIO(volatile uint8_t* DDR, volatile uint8_t* PORT, volatile uint8_t* PIN, uint8_t PINNUM);	/// Create a GPIO pin

GPIO* copyGPIO(GPIO const* in);													/// Copy a GPIO pin

void freeGPIO (GPIO* in);														/// Free (delete) a GPIO pin
