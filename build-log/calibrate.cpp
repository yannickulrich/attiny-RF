#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "../serial.h"
int main()
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
ISR(PCINT0_vect)
{
    cli();
    if (PINB & (1 << PB3)) // Rising
    {
        TxByte(TCNT0);
    }
    TCNT0 = 0;
}
