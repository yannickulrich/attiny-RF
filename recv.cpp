#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "serial.h"

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
    
    TCCR0B |= (1<<CS02) | (1<<CS00);  //prescale timer
    TIMSK0 |= (1<<TOIE0); //enable timer overflow interrupt
    
    // PB3 is the input
    DDRB &= ~(1 << PB3);
    
    // Set timer on PB3
    PCMSK |= (1<<PCINT3);    //  tell pin change mask to listen to D3
    GIMSK |= (1<<PCIE  );     // enable PCINT interrupt in the general interrupt mask

    sei(); 
    while(1);
}

#define DIFF(x,y) ((x<y)?(y-x):(x-y))
#define I_SHORT 3
#define I_LONG 10
#define I_SYNC 105
#define CLEAR_COMMAND { \
    length = 0;\
    command = 0; \
}


uint32_t command;
uint8_t length;


void handleCommand()
{
    SEND_32(command);
    TxByte(length);
}



ISR(TIM0_OVF_vect)
{  
    CLEAR_COMMAND;
}
ISR(PCINT0_vect)
{
    cli();
    if (PINB & (1 << PB3)) // Rising
    {
        if (DIFF(TCNT0, I_SHORT) < 2) // 1
        {
            #ifdef DEBUG
            TxByte('1');
            #endif
            command <<= 1;
            command |= 1;
            length++;
        }
        else if (DIFF(TCNT0, I_LONG) < 2)
        {
            #ifdef DEBUG
            TxByte('0');
            #endif
            command <<= 1;
            length++;
        }
        else if (DIFF(TCNT0, I_SYNC) < 10)
        {
            #ifdef DEBUG
            TxByte('S');
            #endif
            handleCommand();
        }
        else
        {
            #ifdef DEBUG
            TxByte('?');
            TxByte(TCNT0);
            #endif
            
            CLEAR_COMMAND;
        }
        if (command & 0b10000000000000000000000000000000)
        {
            #ifdef DEBUG
            TxByte('R');
            #endif
            CLEAR_COMMAND;
        }
        
    }
    
    TCNT0 = 0;
    
    sei();
}