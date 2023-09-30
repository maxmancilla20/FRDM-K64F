/*
 * Functions.c
 *
 *  Created on: 31 ago. 2023
 *      Author: USER
 */
#include "stdint.h"
#include "Functions.h"

STATES NextState;


STATES NewNextState(FIFO xTask)
{
    return xTask.TaskNum;
}
