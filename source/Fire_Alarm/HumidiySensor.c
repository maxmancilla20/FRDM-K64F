/*
 * HumidiySensor.c
 *
 *  Created on: 3 mar. 2024
 *      Author: USER
 */

#include "HumidiySensor.h"

char* HumiditySimulation(void)
{
    static char humidityString[100];  // Maximum length adjusted to 100

    // Generate a controlled change in humidity
    static int currentHumidity = 85;  // Start at 85
    static int counter = 0;  // Counter to track consecutive calls
    static int increment = 2;  // Increment value during the gradual change
    static uint8_t incrementFlag = 0;
    static uint8_t decrementFlag = 1;

    // Gradual increment to reach 85
    if ((currentHumidity <= 85) && (1 == incrementFlag)) 
    {
        currentHumidity += increment;
    } 
    else if ((currentHumidity >= 85) && (1 == incrementFlag) && (5 >= counter))
    {
        // Hold at 85 for 5 calls
        counter++;
    }
    else if((currentHumidity >= 85) && (1 == incrementFlag) && (5 <= counter))
    {
        /* Changing context to decrement. */
        incrementFlag = 0;
        decrementFlag = 1;
        counter = 0;
    }
    else if ((currentHumidity >= 15) && (1 == decrementFlag)) 
    {
        // Gradual decrement to 15
        currentHumidity -= increment;
    }
    else if((currentHumidity <= 15) && (1 == decrementFlag) && (5 >= counter))  
    {   
        // Hold at 15 for 5 calls
        counter++;
    }
    else if ((currentHumidity <= 15) && (1 == decrementFlag) && (5 <= counter))
    {
        // Reset counters and cycle
        incrementFlag = 1;
        decrementFlag = 0;
        counter = 0;
    }
    else
    {
        /* Nothing */
    }

    // Convert the integer humidity to a string using snprintf
    int charsWritten = snprintf(humidityString, sizeof(humidityString), "%d", currentHumidity);

    // Return the pointer to the static buffer
    return humidityString;
}
