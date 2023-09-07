/*
 * Functions.c
 *
 *  Created on: 31 ago. 2023
 *      Author: USER
 */
#include "stdint.h"
#include "Functions.h"

extern unsigned long Counter;
extern unsigned long CurrentCounter;
extern volatile bool g_ButtonPress;
extern volatile bool g_ButtonPress_TWO;
extern uint16_t Interval;
STATES NextState;

STATES NextStateF(void)
{   
    if (g_ButtonPress)/*when switch 3 is pressed*/
    {
        if(STATE_6 > NextState) /*Threshold to avoid move more than state 6*/
        {
            NextState++; /*Go to the next stat*/
        }        
        g_ButtonPress = false; /*Reset ButtnPress*/
    }

    if ( g_ButtonPress_TWO)
    {
        if(STATE_1 < NextState)/*Threshold to avoid move less than state 1*/
        {
            NextState--; /*Go to a previous state*/
        }
        
        g_ButtonPress_TWO = false; /*Reset ButtnPress*/
    }
    
    return NextState;
}

