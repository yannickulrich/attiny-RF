#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "serial.h"
#include "pulses.h"




int main(void)
{
    cli();
    index = 0;
    status = 0b10000000;
    //prescale timer
    TCCR0B |= (1<<CS00);
    //enable timer overflow interrupt       
    TIMSK0 |= (1<<TOIE0);
    
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



