/*
 * misc.h
 *
 * Created: 14-Feb-2020
 * Last Modified: 23-Feb-2020
 * Author: tejas_advait
 *
 * Description: Miscellaneous functions.
 */

#pragma once


/*
 * Function:  delayMicrosecondsHard [unsigned int -> void]
 * --------------------
 * Causes a delay by busy-wait for the prescribed number of microseconds.
 *
 * howLong: number of microseconds to delay for [unsigned int].
 *
 * returns: void.
 */
void delayMicrosecondsHard (unsigned int howLong); //Microsecond delay