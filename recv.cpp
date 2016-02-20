#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "serial.h"
#include "pulses.h"


#define DIFF(x,y) ((x<y)?(y-x):(x-y))


#define I_SHORT  4 //    350u
#define I_LONG  14 //  1 050u
#define I_SYNC 134 // 10 850u

#define I_TOLERANCE 5

/* 
 * Sync bit: I_SHORT, I_SYNC
 *      _
 *     | |_______________________________
 * "0" bit:  I_SHORT, I_LONG
 *      _
 *     | |___
 * "1" bit:  I_LONG , I_SHORT
 *      ___
 *     |   |_
 */


int main(void)
{
    cli();
    status = 0b10000000;

    OCR0A = 100;	       // Count up to..
    TCNT0 = 0;		       // Counter value
    TIMSK0 |= (1<<OCIE0A); //enable compare match interrupt
    TCCR0A = (1<<WGM01);   //CTC
    TCCR0B |= (1<<CS00);   //prescale timer

    
    // PB3 is the input
    DDRB &= ~(1 << PB3);
    
    // Set timer on PB3
    PCMSK |= (1<<PCINT3);    //  tell pin change mask to listen to D3
    GIMSK |= (1<<PCIE  );     // enable PCINT interrupt in the general interrupt mask

    sei(); 
    
	while(1)
	{
        
        _delay_ms(10);
        if IS_ON(P_FAIL)
        {
            TURN_OFF(N_FAIL)
            TxByte('F');
            
        }
        
        if IS_ON(P_PROC)
        {
            
            if (DIFF(oldHigh, I_SHORT) < I_TOLERANCE)
            {
                if (DIFF(oldLow, I_SYNC) < I_TOLERANCE) // Sync bit
                {
                    TxByte('S');
                }
                else if (DIFF(oldLow, I_LONG) < I_TOLERANCE) // Zero
                {
                    TxByte('0');
                }
                else
                    TxByte('?');
            }
            else if (  (DIFF(oldHigh, I_LONG) < I_TOLERANCE) && (DIFF(oldLow, I_SHORT) < I_TOLERANCE)  ) // One
            {
                 TxByte('1');
            }
            else
                TxByte('/');
            
            SEND_INT(oldHigh);
            SEND_INT(oldLow);
            oldHigh = 0;
            oldLow = 0;

            
            TURN_OFF(N_PROC)

        }
	}
}



