#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>


ISR(TIM0_OVF_vect) {
    PORTB ^=(1<<PB4);
}

ISR(TIM0_COMPA_vect) {
    PORTB ^=(1<<PB4);
}

int main(void)
{
    cli();
    
    TCCR0B |= (1<<CS02) | (1<<CS00);  //prescale timer
    TIMSK0 |= (1<<TOIE0); //enable timer overflow interrupt
    /*
    
    OCR0A = 100;	       // Count up to..
    TCNT0 = 0;		       // Counter value
    TIMSK0 |= (1<<OCIE0A); //enable compare match interrupt
    TCCR0A = (1<<WGM01);   //CTC
    TCCR0B |= (1<<CS00);   //prescale timer
    */

    sei(); 
    
	while(1);
}
