/*
 * misc.c
 *
 * Created: 14-Feb-2020
 * Last Modified: 23-Feb-2020
 * Author: tejas_advait
 *
 * Description: Source code for misc.h.
 */


#include <stdlib.h>
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
