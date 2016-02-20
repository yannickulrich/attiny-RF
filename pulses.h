#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

uint16_t oldHigh, oldLow, high, low;
uint8_t status;

/*
 * STATUS BYTE
 * 
 * b b b b  b b b b
 * ^ ^ ^ ^  ^ ^ ^ ^
 * | | | |  | | | |
 * | | | |  | | | +-- HIGH: Currently in HIGH
 * | | | |  | | +---- LOW : Currently in LOW
 * | | | |  | +------ FAIL: Failed
 * | | | |  +-------- PROC: Received a bit, process in main loop
 * | | | |
 * | | | +-----------
 * | | +-------------
 * | +---------------
 * +-----------------
 */
 


#define TURN_OFF(mask) status &= mask; // Use with N
#define TURN_ON(mask) status |= mask;  // Use with P
#define IS_ON(mask) (status & mask)    // Use with P

#define P_GENERIC 0b00000000
#define N_GENERIC 0b11111111

#define P_HIGH 0b00000001
#define N_HIGH 0b11111110
#define P_LOW  0b00000010
#define N_LOW  0b11111101
#define P_FAIL 0b00000100
#define N_FAIL 0b11111011
#define P_PROC 0b00001000
#define N_PROC 0b11110111

#define FAILED { \
   TURN_ON(P_FAIL) \
   TURN_OFF(N_HIGH) \
   TURN_OFF(N_LOW) \
}

   

ISR(TIM0_COMPA_vect) {

    //overflow occurs at 1024/9600000*256=27.3ms/18.3Hz (measured at 26.7ms/18.75Hz) 
    if IS_ON(P_HIGH) // Currently in HIGH
    {
        high++;
        if (high == 0xfff)
            FAILED
    }
    else if IS_ON(P_LOW) // Currently in LOW
    {
        low++;
        if (low == 0xfff)
            FAILED
    }
}

ISR(PCINT0_vect)
{
    if (PINB & (1 << PB3)) // Rising
    {
        
        if IS_ON(P_LOW)  // We were counting LOWs and are done
        {
            oldHigh = high;
            oldLow = low;
            TURN_ON(P_PROC)
        }
        high = 0;
        TURN_OFF(N_FAIL)
        
        TURN_OFF(N_LOW)
        TURN_ON(P_HIGH)
    }
    else // Falling
    {
        if IS_ON(P_HIGH) // We were counting HIGHs and continue with LOWs
        {
            low = 0;
            TURN_OFF(N_HIGH)
            TURN_ON(P_LOW)
        }
    }
}
