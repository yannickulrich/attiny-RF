#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "serial.h"
#include "pulses.h"




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
        if IS_ON(P_PROC)
        {
            cli();
            TxByte('S');
            
            SEND_INT(oldHigh);
            SEND_INT(oldLow);
            oldHigh = 0;
            oldLow = 0;
            
            TURN_OFF(N_PROC)
            sei();

        }
	}
}



