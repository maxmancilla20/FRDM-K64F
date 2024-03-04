/*
 * TempSensor.c
 *
 *  Created on: 3 mar. 2024
 *      Author: USER
 */
#include "TempSensor.h"

char* TempSimulation(void) {
        static char temperatureString[100];  // Maximum length adjusted to 100

    // Generate a controlled change in temperature
    static int currentTemperature = 15;  // Start at 15
    static int counter = 0;  // Counter to track consecutive calls
    static int increment = 2;  // Increment value during the gradual change
    static uint8_t IncrementFlag = 1;
    static uint8_t DecrementFlag = 0;
    // Gradual increment to reach 85
    if ((currentTemperature <= 85) && (1 == IncrementFlag)) 
    {
        currentTemperature += increment;
    } 
    else if ((currentTemperature >= 85) && (1 == IncrementFlag) && (5 >= counter))
    {
        // Hold at 85 for 5 calls
        counter++;
    }
    else if((currentTemperature >= 85) && (1 == IncrementFlag) && (5 <= counter))
    {
        /*Changing context to decrement.*/
        IncrementFlag = 0;
        DecrementFlag = 1;
        counter = 0;
    }
    else if ((currentTemperature >= 15) && (1 == DecrementFlag) ) 
    {
        // Gradual decrement to 15
        currentTemperature -= increment;
    }
    else if((currentTemperature <= 15) && (1 == DecrementFlag) && (5 >= counter))  
    {   
        // Hold at 85 for 5 calls
        counter++;
    }
    else if ((currentTemperature <= 15) && (1 == DecrementFlag) && (5 <= counter))
    {
        // Reset counters and cycle
        IncrementFlag = 1;
        DecrementFlag = 0;
        counter = 0;
    }
    else
    {
        /*nothing*/
    }

    // Convert the integer temperature to a string using snprintf
    int charsWritten = snprintf(temperatureString, sizeof(temperatureString), "%d", currentTemperature);

    // Return the pointer to the static buffer
    return temperatureString;
}


