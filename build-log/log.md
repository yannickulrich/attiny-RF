# Build log
This is to document problems and issues arising while designing this code.

## The problem
The output of a cheap RF receiver is decoded using an ATtiny13. The expected signals are defined as Protocol 1 in the Arduino library [rc-switch](https://github.com/sui77/rc-switch). This means the pulse look like this
```
SYNC bit: SHORT, SYNC = 1, 31
     _
    | |_______________________________
"0" bit:  SHORT, LONG = 1,  3
     _
    | |___
"1" bit:  LONG, SHORT = 3,  1
     ___
    |   |_
```
The `SHORT` pulse has a length of 350us.

## Getting serial to work
The first problem is the serial connection. The ATtiny does not have an hardware serial. Therefore, a bit-banging software serial is required. [Ralph Doncaster's](http://forum.arduino.cc/index.php?topic=207467.0) code provides a great starting point. However, he only supports 8 and 16 MHz processors while the ATtiny has either 1.2MHz (CKDIV8 set) or 9.6MHz (CKDIV8 unset). That means one needs to find a value for `TXDELAY` experimentally. By sending 0x55 (0b01010101) every 10ms one can measure the pulse length of one bit on TP1
```C++
#include "serial.h"

int main(void)
{
	while(1)
	{
        _delay_ms(10);
        TxByte(0x55);
	}
}
```
For 9600baud (115200baud) one expects 104us (8.68us). Now one tunes `TXDELAY` till the pulse length is correct. One finds
```C++
#if F_CPU == 1200000
#define TXDELAY 38 // 9600 Baud
#elif F_CPU == 9600000L
#define TXDELAY 25 // 115200 Baud
#else
#error
#endif
```
For the following we will be using 115200baud at 9.6MHz.

## Timing
For RF decoding precise timing is crucial. AVR microcontrollers have to ways to achieve this
* CTC (Clear Timer on Compare): The counter counts up to specified value and calls the interrupt (ISR) once it is reached. Example (`blink.cpp`):
```C++
OCR0A = 100;	       // Count up to..
TCNT0 = 0;		       // Counter value
TIMSK0 |= (1<<OCIE0A); //enable compare match interrupt
TCCR0A = (1<<WGM01);   //CTC
TCCR0B |= (1<<CS00);   //prescale timer
```
* Overflow: Once the timer reaches 255, the ISR is called
```C++
TCCR0B |= (1<<CS02) | (1<<CS00);  //prescale timer
TIMSK0 |= (1<<TOIE0); //enable timer overflow interrupt
```
The next issue is the choice of the prescale. A prescale of 1024 ([setting both CS02 and CS00](https://sites.google.com/site/qeewiki/books/avr-guide/timer-on-the-atmega8)) was chosen. This means that the counter `TCNT0` is increased every 1024/9.6MHz = 107us which is sufficient since the smallest pulse that needs to be detected is 350us long.

## Pulse detection
The central part of an RF decoder is, of course, the detection of the pulses. This is done by an ISR on the input, i.e. PB3. The following code is used to measure the times of a pulse (`calibrate.cpp`)
```C++
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "serial.h"
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
```
The Arduino sketch `rc_test.ino` sends a series of pulses. Using e.g. python one can decode the timings and establish the following relations
```C++
#define I_SHORT 3
#define I_LONG 10
#define I_SYNC 105
```
Now the ISR can easily be completed
```C++
#define DIFF(x,y) ((x<y)?(y-x):(x-y))
ISR(PCINT0_vect)
{
    cli();
    if (PINB & (1 << PB3)) // Rising
    {
        if      (DIFF(TCNT0, I_SHORT) <  2) // 1 bit
            TxByte('1');
        else if (DIFF(TCNT0, I_LONG ) <  2) // 0 bit
            TxByte('0');
        else if (DIFF(TCNT0, I_SYNC ) < 10) // SYNC bit
            TxByte('S');
        else
        {
            TxByte('?');
            TxByte(TCNT0);
        }
    }
    TCNT0 = 0;
    sei();
}
```

## Assembly of the code word
In order to turn the series of 1s and 0s into a code word, an efficient assembly routine is needed:
* Most commercial devices use 24bit codewords, i.e. `uint32_t command` stores the code word
* If a 1 or a 0 is received, the code word is shifted one bit, i.e. `command <<= 1;`
* If the received bit was a 1, the corresponding bit is set, i.e. `command |= 1;`
* If a SYNC bit is received, the code word is completed and can be analysed. Afterwards it is cleared.
* If the timer overflows or something else is received, i.e. noise, the code word is cleared.
* If the code word is about to flow over, i.e. the next bit would exceed the 32bit variable (`command & 0b10000000000000000000000000000000`) the code word is cleared.
