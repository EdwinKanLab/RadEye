#include <stdio.h>
#include "PiGPIO.h"

#include <sys/time.h>
#include <time.h>

void delayMicrosecondsHard (unsigned int howLong)
{
  struct timeval tNow, tLong, tEnd ;

  gettimeofday (&tNow, NULL) ;
  tLong.tv_sec  = howLong / 1000000 ;
  tLong.tv_usec = howLong % 1000000 ;
  timeradd (&tNow, &tLong, &tEnd) ;

  while (timercmp (&tNow, &tEnd, <))
    gettimeofday (&tNow, NULL) ;
}

void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

int main(){
	printf("Hello world!\n");
	

	PiGPIO* DNPIN = createPiGPIO(19);

	DNPIN->setAsInput(DNPIN);

	uint8_t val;

	while (1){
		val = DNPIN->read(DNPIN);
		delayMicrosecondsHard(1000000);
		printf("DNPIN Val: %d \n", val);

	}
	return 0;
}
