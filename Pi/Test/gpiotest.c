#include "PiGPIO.h"
#include <unistd.h>
#include <sys/time.h>

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

int main(){
	PiGPIO* pin19 = createPiGPIO(19);
	
	pin19->setAsOutput(pin19);


	while (1){
		pin19->writeHigh(pin19);

		delayMicrosecondsHard(100000);

		pin19->writeLow(pin19);

		delayMicrosecondsHard(500000);
	}


	return 0;

}
