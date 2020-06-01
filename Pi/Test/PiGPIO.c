/*
 * PiGPIO.c
 *
 * Created: 12-Feb-20
 * Last Modified: 23-Feb-2020
 * Author: tejas_advait
 *
 * Description: Source code for PiGPIO.h.
 */ 


#define BCM2837_PERI_BASE        0x3F000000
#define GPIO_BASE                (BCM2837_PERI_BASE + 0x200000)

#include "PiGPIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

static volatile uint32_t * setup_io()
{
   /* open /dev/mem */
	int  mem_fd;
	void *gpio_map;
	volatile uint32_t *gpio;

	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
		printf("can't open /dev/mem \n");
      	exit(-1);
   }

   /* mmap GPIO */
   	gpio_map = mmap(
   	  NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

    if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);//errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile uint32_t *)gpio_map;
   return gpio;
} // setup_io

static void setAsInput(PiGPIO* self){

	*(self->setupGPIO + (self->PINNUM/10)) &= ~(7 << ((self->PINNUM % 10)*3));
}

static void setAsOutput(PiGPIO* self){
	self->setAsInput(self);

	*(self->setupGPIO + (self->PINNUM/10)) |= (1 << ((self->PINNUM % 10)*3));

} 

static void writeHigh(PiGPIO* self){
	*(self->setupGPIO + 7) = (1<<self->PINNUM);
}

static void writeLow(PiGPIO* self){
	*(self->setupGPIO + 10) = (1<<self->PINNUM);

}

static uint8_t readPin(PiGPIO* self){ 

	return (uint8_t) (((*(self->setupGPIO+13)&(1<<self->PINNUM)) >> (self->PINNUM)) & 0x00000001);
}

PiGPIO* createPiGPIO(uint32_t PINNUM){

	PiGPIO* gpio = (PiGPIO*) malloc(sizeof(PiGPIO));
	if (gpio == NULL){
		/*Handle malloc failure*/
		;
	}

	volatile uint32_t * setupGPIO = setup_io();
	
	*gpio = (PiGPIO){.PINNUM=PINNUM,.setupGPIO=setupGPIO,
			.setAsInput=setAsInput, .setAsOutput=setAsOutput,
			.writeHigh=writeHigh, .writeLow=writeLow,
			.read=readPin};
			
	return gpio;
}

PiGPIO* copyGPIO(PiGPIO const* in){
	PiGPIO* out = malloc(sizeof(PiGPIO));
	*out = *in;
	return out;
}

void freePiGPIO(PiGPIO* in){ free(in); }
