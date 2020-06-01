/*
 * PiComm.h
 *
 * Created: 14-Feb-20
 * Last Modified: 23-Feb-2020
 * Author: tejas_advait
 *
 * Description: Mini library for communication between Raspberry Pi and uC.
 */ 

#pragma once

#include <stdint.h>

#include "PiGPIO.h"


/*
 * Struct: PiComm 
 * --------------------
 * Provides an abstraction for communication between Raspberry Pi and uC.
 *
 * Fields: ENPIN [PiGPIO*] - Enable Pin.
 *         
 *         DNPIN [PiGPIO*] - Done Pin.
 *
 *         DATPORT [PiGPIO**] - Data Port.
 *
 *         numCycles [uint8_t] - Number of cycles.
 * 
 *         DATASINGLE [uint8_t] - Single data for writing.
 *
 *         DATA [uint8_t*] - Pointer to an array of data for writing.
 *
 * Methods: readDn       [PiComm* -> uint8_t] - Read Done Pin.
 *
 *          writeSingle  [PiComm* -> void] - Write the Data Port once.
 *
 *          writeCycle   [PiComm* -> void] - Write the data port numCycles times.
 *
 *          setEnHigh    [PiComm* -> void] - Set Enable Pin to high.
 *
 *          setEnLow     [PiComm* -> void] - Set Enable Pin to low.
 *
 *          setData      [PiComm* -> int16_t* -> void] - Set Data from rotations.
 *
 */
typedef struct PiComm {
    PiGPIO* ENPIN;
    PiGPIO* DNPIN;

    PiGPIO** DATPORT;

    uint8_t numCycles;

    uint8_t DATASINGLE; 
    uint8_t* DATA;
    

    uint8_t (*readDn)(struct PiComm*);
    void (*writeSingle)(struct PiComm*);
    void (*writeCycle)(struct PiComm*);
    void (*setEnHigh)(struct PiComm*);
    void (*setEnLow)(struct PiComm*);
    void (*setData)(struct PiComm*, int16_t* rots);


  } PiComm;



/*
 * Function:  createPiGPIO [uint32_t -> PiGPIO*]
 * --------------------
 * Creates a PiComm*.
 *
 * ENPIN: Enable Pin. [PiGPIO*]
 *         
 * DNPIN: Done Pin. [PiGPIO*]
 *
 * DATPORT: Data Port. [PiGPIO**]
 *
 * numCycles: Number of cycles. [uint8_t]
 * 
 * DATASINGLE: Single data for writing. [uint8_t]
 *
 * DATA: Pointer to an array of data for writing. [uint8_t*]
 *
 * returns: Pointer to a PiComm struct [PiComm*].
 */
PiComm* createPiComm(PiGPIO* ENPIN, PiGPIO* DNPIN, PiGPIO** DATPORT, uint8_t numCycles, uint8_t* DATA);


/*
 * Function:  copyPiComm [PiComm const* -> PiComm*]
 * --------------------
 * Creates a deep copy of the input PiComm*.
 *
 * in: Input. [PiComm const*]
 *
 * returns: Pointer to a PiComm struct [PiComm*].
 */
PiComm* copyPiComm(PiComm const* in);


/*
 * Function:  freePiComm [PiComm* -> void]
 * --------------------
 * Frees the memory associated with the input PiComm*.
 *
 * in: Input. [PiComm*]
 *
 * returns: void.
 */
void freePiComm(PiComm* in);
