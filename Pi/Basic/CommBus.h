/*
 * CommBus.h
 *
 * Created: 02-Jan-2020
 * Last Modified: 08-Feb-2020
 * Author : tejas_advait
 */

#pragma once

#include <stdint.h>

#include "GPIO.h"
#include "GPIORT.h"



typedef struct CommBus {
    GPIO* ENPIN; //Enable Pin
    GPIO* DNPIN; //Done Pin

    GPIORT* DATPORT; //Data Port

    uint8_t numCycles; //Number of cycles
    

    uint8_t (*readEn)(struct CommBus*); //Read enable pin
    void (*readSingle)(struct CommBus*); //Read the data port once
    void (*readCycle)(struct CommBus*); //Read the data port numCycles times
    void (*setDnHigh)(struct CommBus*); //Set done pin to high
    void (*setDnLow)(struct CommBus*); //Set done pin to low
    uint8_t DATASINGLE; //Stores single data from port
    uint8_t DATA[]; //Stores cycle data from port

  }  CommBus;




CommBus* createCommBus(GPIO* ENPIN, GPIO* DNPIN, GPIORT* DATPORT, uint8_t numCycles);

CommBus* copyCommBus(CommBus const* in);

void freeCommBus(CommBus* in);